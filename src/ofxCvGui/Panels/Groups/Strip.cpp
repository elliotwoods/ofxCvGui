#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			//----------
			Strip::Strip(Direction direction) :
			minimumCellSize(10),
			borderSize(8) {
				this->direction = direction;

				this->borders = make_shared<ElementGroup>();
				this->borders->addListenersToParent(this, true);

				this->onBoundsChange += [this](BoundsChangeArguments& args) {
					this->boundsChange(args);
				};

				this->onUpdate += [this](UpdateArguments & args) {
					if (this->needsArrange) {
						this->arrange();
					}
				};

				this->onGroupChange.addListener([this]() {
					auto cellCount = this->getElements().size();
					this->programmaticCellSizes.resize(cellCount, -1);

					this->rebuildBorders();
				}, this);
					
				this->setHandlesEnabled(false);
			}

			//----------
			void Strip::setDirection(Direction direction) {
				this->direction = direction;
			}

			//----------
			Strip::Direction Strip::getDirection() const {
				return this->direction;
			}

			//----------
			void Strip::toggleDirection() {
				if (this->direction == Vertical) {
					this->setDirection(Horizontal);
				}
				else {
					this->setDirection(Vertical);
				}
				this->arrange();
			}

			//----------
			void Strip::setCellSizes(const vector<int> & cellSizes) {
				this->programmaticCellSizes = vector<int>(cellSizes.begin(), cellSizes.end());
				this->resetUserCellSizes();
			}

			//----------
			void Strip::setUserCellSize(size_t index, int size) {
				while (index >= this->userCellSizes.size()) {
					this->userCellSizes.push_back(-1);
				}
				this->userCellSizes[index] = size;
				this->arrange();
			}

			//----------
			int Strip::getCellSize(size_t index) const {
				if (index >= this->cachedCellSizes.size()) {
					return 0;
				}
				else {
					return this->cachedCellSizes[index];
				}
			}

			//----------
			void Strip::setHandlesEnabled(bool handlesEnabled) {
				this->handlesEnabled = handlesEnabled;
				this->needsArrange = true;
			}

			//----------
			void Strip::resetUserCellSizes() {
				this->userCellSizes = this->programmaticCellSizes;
				this->needsArrange = true;
			}

			//----------
			void Strip::boundsChange(BoundsChangeArguments & args) {
				const auto count = this->elements.size();

				//avoid any divide by zeros
				if (count == 0) {
					return;
				}

				//ensure we have a size parameter for each cell
				auto cellSizes = this->userCellSizes;
				cellSizes.resize(count, -1);

				//calculate actual widths (minus minimumCellSize)
				{
					float totalSpace = this->direction == Direction::Horizontal ? args.localBounds.width : args.localBounds.height;
					
					//remove minimumCellSize's from totalSpace
					totalSpace -= count * this->minimumCellSize;

					//strip minimum sizes from cell sizes, calculate total predetermined space
					int predeterminedSpace = 0;
					int undeterminedCount = 0;
					for (auto & cellSize : cellSizes) {
						if (cellSize != -1) {
							cellSize -= 10;
							cellSize = max(cellSize, 0);
							predeterminedSpace += cellSize;
						}
						else {
							undeterminedCount++;
						}
					}

					//is total predetermined (+minimumSize per undetermined) more than is available?
					float availabilityFactor = totalSpace / (float)(predeterminedSpace);
					bool needsShrink = availabilityFactor < 1.0f;
					bool needsExpand = undeterminedCount == 0 && availabilityFactor > 1.0f;

					float undeterminedCellSize = needsShrink ? 0.0f : (float) (totalSpace - predeterminedSpace) / (float) undeterminedCount;

					//update the cell sizes and re-add the minimum
					for (auto & cellSize : cellSizes) {
						if (cellSize == -1) {
							cellSize = undeterminedCellSize + this->minimumCellSize;
						}
						else {
							if (needsShrink || needsExpand) {
								cellSize *= availabilityFactor;
							}
							cellSize += this->minimumCellSize;
						}
					}
				}

				//cache the cell sizes
				this->cachedCellSizes = cellSizes;

				//apply sizes
				if (this->direction == Direction::Horizontal) {
					float x = 0.0f;
					for (size_t i = 0; i < count; i++) {
						auto cellSize = cellSizes[i];
						this->elements[i]->setBounds(ofRectangle(x, 0, cellSize, args.localBounds.height));
						x += cellSize;
					}
				} else  {
					float y = 0.0f;
					for (size_t i = 0; i < count; i++) {
						auto cellSize = cellSizes[i];
						this->elements[i]->setBounds(ofRectangle(0, y, args.localBounds.width, cellSize));
						y += cellSize;
					}
				}

				this->layoutBorders();
				this->needsArrange = false;
			}

			//----------
			void Strip::rebuildBorders() {
				if (this->handlesEnabled) {
					auto count = this->elements.size();
					
					for (size_t i = 0; i < count - 1; i++) {
						auto border = make_shared<Element>();
						auto borderWeak = weak_ptr<Element>(border);

						//generic draw
						border->onDraw += [borderWeak](DrawArguments & args) {
							auto border = borderWeak.lock();
							if (!border) {
								return;
							}

							ofPushStyle();
							{
								if (border->isMouseDown() || border->isMouseDragging()) {
									ofFill();
									ofSetLineWidth(0);
									ofSetColor(100);
									ofDrawRectangle(args.localBounds);
								} else if (border->isMouseOver()) {
									ofFill();
									ofSetLineWidth(0);
									ofSetColor(50);
									ofDrawRectangle(args.localBounds);

									ofxCvGui::Utils::drawToolTip("Resize", args.localBounds.getCenter());
								}
							}
							ofPopStyle();
						};

						//specific action (different movement)
						{
							auto action = [this, i, borderWeak](float movement, MouseArguments & mouseArgs) {
								auto border = borderWeak.lock();
								if (!border) {
									return;
								}

								mouseArgs.takeMousePress(border);

								if (mouseArgs.isDragging(border) && mouseArgs.button == 0) {
									auto prevCellHeight = this->getCellSize(i);
									auto nextCellHeight = this->getCellSize(i + 1);

									prevCellHeight += movement; //**
									nextCellHeight -= movement; //**

									bool prevIsAuto = this->programmaticCellSizes[i] == -1;
									bool nextIsAuto = this->programmaticCellSizes[i + 1] == -1;
									auto bothAreAuto = prevIsAuto && nextIsAuto;

									if (bothAreAuto || nextIsAuto) {
										this->setUserCellSize(i, prevCellHeight);
									}
									if (bothAreAuto || prevIsAuto) {
										this->setUserCellSize(i + 1, nextCellHeight);
									}
								}
								if (mouseArgs.isDoubleClicked(border)) {
									this->toggleDirection();
								}
							};
							border->onMouse += [action, this](MouseArguments& args) {
								if (this->direction == Direction::Horizontal) {
									action(args.movement.x, args);
								}
								else {
									action(args.movement.y, args);
								}
								};
						}

						//generic action (reset)
						border->onMouseReleased += [this, borderWeak](MouseArguments & args) {
							if (args.button == 2) {
								this->resetUserCellSizes();
							}
						};
						this->borders->add(border);
					}
					this->layoutBorders();
				}
				else {
					this->borders->clear();
				}
			}

			//----------
			void Strip::layoutBorders() {
				if (this->direction == Direction::Horizontal) {
					float x = - (float) this->borderSize / 2.0f;
					auto height = this->getHeight();
					for (size_t i = 0; i < this->borders->getElements().size(); i++) {
						x += this->getCellSize(i);
						ofRectangle bounds(x, 0, this->borderSize, height);
						this->borders->getElements()[i]->setBounds(bounds);
					}
				}
				else {
					float y = - (float) this->borderSize / 2.0f;
					auto width = this->getWidth();
					for (size_t i = 0; i < this->borders->getElements().size(); i++) {
						y += this->getCellSize(i);
						ofRectangle bounds(0, y, width, this->borderSize);
						this->borders->getElements()[i]->setBounds(bounds);
					}
				}
			}

			//----------
			shared_ptr<Strip> makeStrip(Strip::Direction direction) {
				return make_shared<Strip>(direction);
			}
		}
	}
}