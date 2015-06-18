#include "ofxCvGui/Panels/World.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		World::World() {
			this->gridColor = ofColor(90, 50, 50);
			this->gridEnabled = true;
			this->gridLabelsEnabled = true;
			this->gridScale = 10.0f;

			this->useFbo = false; //default should be false, else we may add listeners twice to the camera

			this->onUpdate += [this](UpdateArguments & args) {
				if (this->useFbo) {
					ofEventArgs dummyArgs;
					this->camera.update(dummyArgs);
				}
			};

			this->onDraw += [this](DrawArguments & args) {
				if (this->useFbo) {
					if (this->fbo.isAllocated()) {
						bool scissorEnabled = Utils::disableScissor();

						this->fbo.begin();
						ofClear(100);

						this->drawContent(args.localBounds);

						this->fbo.end();

						if (scissorEnabled) {
							Utils::enableScissor();
						}
						this->fbo.draw(0, 0);
					}
				} else {
					this->drawContent(args.globalBounds);
				}
			};

			this->onMouse += [this](MouseArguments & args) {
				args.takeMousePress(this);

				if (this->useFbo) {
					this->fbo.bind();

					args.takeMousePress(this);

					ofMouseEventArgs ofArgs;
					(ofVec2f&)ofArgs = args.local;
					ofArgs.button = args.button;

					switch (args.action) {
					case MouseArguments::Action::Moved:
						ofArgs.type = ofMouseEventArgs::Type::Moved;
						this->camera.mouseMoved(ofArgs);
						break;
					case MouseArguments::Action::Pressed:
						ofArgs.type = ofMouseEventArgs::Type::Pressed;
						this->camera.mousePressed(ofArgs);
						break;
					case MouseArguments::Action::Released:
						ofArgs.type = ofMouseEventArgs::Type::Released;
						this->camera.mouseReleased(ofArgs);
						break;
					case MouseArguments::Action::Dragged:
						ofArgs.type = ofMouseEventArgs::Type::Dragged;
						this->camera.mouseDragged(ofArgs);
						break;
					default:
						break;
					}

					this->fbo.unbind();
				}
			};

			this->onKeyboard += [this](KeyboardArguments & args) {
				if (this->useFbo) {
					this->fbo.bind();

					ofKeyEventArgs ofArgs;
					ofArgs.key = args.key;
					if (args.action == KeyboardArguments::Action::Pressed) {
						ofArgs.type = ofKeyEventArgs::Type::Pressed;
						this->camera.keyPressed(ofArgs);
					}
					else if (args.action == KeyboardArguments::Action::Released) {
						ofArgs.type = ofKeyEventArgs::Type::Pressed;
						this->camera.keyReleased(ofArgs);
					}

					this->fbo.unbind();
				}
			};

			this->onBoundsChange += [this](BoundsChangeArguments & args) {
				if (this->useFbo) {
					this->allocateFbo();
				}
			};
		}

		//----------
		void World::drawContent(const ofRectangle & bounds) {
			this->camera.begin(bounds);

			if (this->gridEnabled) {
				ofPushStyle();
				ofSetColor(this->gridColor);
				ofDrawGrid(gridScale, 5.0f, this->gridLabelsEnabled);
				ofPopStyle();
			}

			vector<ofNode*>::iterator it;
			for (it = this->nodes.begin(); it != this->nodes.end(); it++)
				(**it).draw();
			this->onDrawWorld.notifyListeners(this->camera);

			this->camera.end();
		}

		//----------
		void World::allocateFbo() {
			const auto bounds = this->getLocalBounds();
			ofFbo::Settings fboSettings;
			fboSettings.width = bounds.width;
			fboSettings.height = bounds.height;
			fboSettings.useDepth = true;

			this->fbo.allocate(fboSettings);
		}

		//----------
		void World::setGridEnabled(bool gridEnabled) {
			this->gridEnabled = gridEnabled;
		}
		
		//----------
		void World::setGridColor(const ofColor & gridColor) {
			this->gridColor = gridColor;
		}
		
		//----------
		void World::setGridScale(const float gridScale) {
			this->gridScale = gridScale;
		}

		//----------
		void World::setGridLabelsEnabled(bool gridLabelsEnabled) {
			this->gridLabelsEnabled = gridLabelsEnabled;
		}

		//----------
		void World::setUseFbo(bool useFbo) {
			//since there's a lot of state associated with this
			//don't allow setting if already set
			if (useFbo == this->useFbo) {
				return;
			}

			this->useFbo = useFbo;
			if (useFbo) {
				this->camera.removeListeners();
				this->allocateFbo();
			}
			else {
				this->camera.addListeners();
			}
		}

		//----------
		bool World::getUseFbo() const {
			return this->useFbo;
		}
	}
}