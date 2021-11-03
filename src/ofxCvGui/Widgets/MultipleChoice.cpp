#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		const float MultipleChoice::radius = 4.0f;

		//----------
		MultipleChoice::MultipleChoice(const string & caption) {
			this->setCaption(caption);

			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				//draw caption
				auto & captionFont = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 12);
				captionFont.drawString(this->caption + " : ", 0, 15);
			};
			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments & args) {
				auto optionsBounds = args.localBounds;
				optionsBounds.x = 0.0f;
				optionsBounds.y = 20.0f;
				optionsBounds.width = args.localBounds.width - 10.0f;
				optionsBounds.height = args.localBounds.height - 30.0f;
				this->optionsElement->setBounds(optionsBounds);
			};

			this->addChild(this->optionsElement);
			this->optionsElement->onDraw += [this](ofxCvGui::DrawArguments& args) {
				//draw background to all elements
				ofPushStyle();
				{
					ofSetColor(50);
					ofDrawRectRounded(args.localBounds, MultipleChoice::radius);
				}
				ofPopStyle();
			};
			this->optionsElement->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments& args) {
				auto optionBounds = args.localBounds;
				optionBounds.width /= (float)this->options.size();

				const float borderSize = 2.0f;
				optionBounds.width -= borderSize * 2.0f;
				optionBounds.x += borderSize;
				optionBounds.height -= borderSize * 2.0f;
				optionBounds.y += borderSize;

				auto optionElements = optionsElement->getChildren();
				for (auto optionElement : optionElements) {
					optionElement->setBounds(optionBounds);
					optionBounds.x += optionBounds.width + borderSize * 2.0f;
				}
			};
			this->optionsElement->onMouse += [this](ofxCvGui::MouseArguments& args) {
				// Take the mouse press for the group element
				args.takeMousePress(this->optionsElement);

				switch (args.action) {
				case MouseArguments::Action::Dragged:
				case MouseArguments::Action::Pressed:
				case MouseArguments::Action::Released:
				{
					auto selectedIndex = args.local.x * this->options.size() / this->optionsElement->getWidth();
					this->setSelection(selectedIndex);
					break;
				}
				default:
					break;
				}
			};

			this->setBounds(ofRectangle(0, 0, 100, 60));
		}

		//----------
		MultipleChoice::MultipleChoice(const string & caption, const initializer_list<string> & options)
			: MultipleChoice(caption) {
			this->addOptions(options);
		}

		//----------
		MultipleChoice::MultipleChoice(const string& caption, const vector<string>& options)
			: MultipleChoice(caption) {
			this->addOptions(options);
		}

		//----------
		void MultipleChoice::addOption(string choice) {
			this->options.push_back(choice);
			this->rebuildOptions();
		}


		//----------
		void MultipleChoice::addOptions(vector<string> options) {
			for (const auto & option : options) {
				this->options.push_back(option);
			}
			this->rebuildOptions();
		}

		//----------
		void MultipleChoice::removeOption(string option) {
			auto find = std::find(this->options.begin(), this->options.end(), option);
			if (find == this->options.end()) {
				ofLogError("ofxCvGui::Widgets::MultipleChoice") << "Cannot remove option [" + option + "] since it does not exist in the list of options";
			} else {
				this->options.erase(find);
			}
			this->rebuildOptions();
		}

		//----------
		void MultipleChoice::clearOptions() {
			this->options.clear();
			this->rebuildOptions();
		}

		//----------
		void MultipleChoice::rebuildOptions() {
			this->clampSelection(); // also moves up the selection to 0 if we have allowNullSelection to false 
			this->optionsElement->getChildren().clear();

			for (size_t i = 0; i < this->options.size(); i++) {
				auto optionElement = make_shared<Element>();
				optionElement->onDraw += [this, i](ofxCvGui::DrawArguments& args) {
					if (i == this->selectionIndex) {
						// Draw background if selected
						ofPushStyle();
						{
							ofSetColor(80);
							ofDrawRectRounded(args.localBounds, MultipleChoice::radius);
						}
						ofPopStyle();
					}
				};

				if (this->glyphs.size() > i) {
					// Draw as glyph
					auto glyph = this->glyphs[i];
					optionElement->onDraw += [glyph](ofxCvGui::DrawArguments& args) {
						Utils::drawGlyph(glyph, args.localBounds);
					};
					// Add tooltip with string
					optionElement->addToolTip(this->options[i]);
				}
				else {
					// Draw as text
					auto text = this->options[i];
					optionElement->onDraw += [text](ofxCvGui::DrawArguments& args) {
						Utils::drawText(text, args.localBounds, false);
					};
				}

				this->optionsElement->addChild(optionElement);
			}

			this->arrange();
		}

		//----------
		void MultipleChoice::setSelection(int selectionIndex) {
			if (this->selectionIndex == selectionIndex) {
				return;
			}

			this->selectionIndex = selectionIndex;
			this->clampSelection();
			if (this->selectionIndex == selectionIndex) {
				this->onValueChange(this->selectionIndex);
			}
		}

		//----------
		void MultipleChoice::setSelection(string choice) {
			auto find = std::find(this->options.begin(), this->options.end(), choice);
			if (find == this->options.end()) {
				ofLogError("ofxCvGui::Widgets::MultipleChoice") << "Cannot select option [" + choice + "] since it does not exist in the list of options";
				this->setSelection(-1);
			} else {
				this->setSelection((int)(find - this->options.begin()));
			}
		}

		//----------
		int MultipleChoice::getSelectionIndex() const {
			return this->selectionIndex;
		}

		//----------
		string MultipleChoice::getSelection() const {
			if (this->selectionIndex == -1) {
				return "";
			} else {
				return this->options[this->getSelectionIndex()];
			}
		}


		//----------
		void MultipleChoice::setAllowNullSelection(bool allowNullSelection) {
			this->allowNullSelection = allowNullSelection;
		}


		//----------
		bool MultipleChoice::getAllowNullSelection() const {
			return this->allowNullSelection;
		}

		//----------
		void MultipleChoice::setGlyphs(const vector<string>& glyphs) {
			this->glyphs = glyphs;
			this->rebuildOptions();
		}

		//----------
		void MultipleChoice::clampSelection() {
			if (this->options.empty()) {
				this->selectionIndex = -1;
			}
			else {
				auto clampedSelection = ofClamp(this->selectionIndex, this->allowNullSelection ? -1 : 0, this->options.size() - 1);
				if (clampedSelection != this->selectionIndex) {
					this->setSelection(clampedSelection);
				}
			}
		}
	}
}