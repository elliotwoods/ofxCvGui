#include "MultipleChoice.h"
#include "ofxAssets.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		MultipleChoice::MultipleChoice(string caption) {
			this->selectionIndex = -1;
			this->setCaption(caption);

			this->setBounds(ofRectangle(0, 0, 100, 60));
			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				//draw caption
				auto & captionFont = ofxAssets::font(ofxCvGui::defaultTypeface, 12);
				captionFont.drawString(this->caption + " : ", 0, 15);

				const auto radius = 4.0f;

				//draw main background
				ofPushStyle();
				ofSetColor(50);
				ofDrawRectRounded(optionsBounds, radius);
				ofPopStyle();

				//draw selection
				ofPushStyle();
				ofSetColor(80);
				ofDrawRectRounded(this->getOptionBounds(this->selectionIndex), radius);
				ofPopStyle();

				//draw options
				for (int optionIndex = 0; optionIndex < this->options.size(); optionIndex++) {
					auto optionBounds = this->getOptionBounds(optionIndex);
					Utils::drawText(this->options[optionIndex], optionBounds, false);
				}

				//draw side line
				ofPushStyle();
				ofSetLineWidth(1.0f);
				ofDrawLine(this->getWidth(), 0, this->getWidth(), this->optionsBounds.getBottom());
				ofPopStyle();
			};
			this->onMouse += [this](ofxCvGui::MouseArguments & args) {
				if (args.takeMousePress(this) || args.isDragging(this) ) {
					for (int i = 0; i < this->options.size(); i++) {
						if (this->getOptionBounds(i).inside(args.local)) {
							this->setSelection(i);
						}
					}
				}
			};
			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments & args) {
				this->optionsBounds = args.localBounds;
				this->optionsBounds.x = 0.0f;
				this->optionsBounds.y = 20.0f;
				this->optionsBounds.width = args.localBounds.width - 10.0f;
				this->optionsBounds.height = args.localBounds.height - 30.0f;
			};
		}

		//----------
		void MultipleChoice::addOption(string choice) {
			this->options.push_back(choice);
			this->clampSelection();
		}

		//----------
		void MultipleChoice::removeOption(string option) {
			auto find = std::find(this->options.begin(), this->options.end(), option);
			if (find == this->options.end()) {
				ofLogError("ofxCvGui::Widgets::MultipleChoice") << "Cannot remove option [" + option + "] since it does not exist in the list of options";
			} else {
				this->options.erase(find);
			}
			this->clampSelection();
		}

		//----------
		void MultipleChoice::clearOptions() {
			this->options.clear();
			this->clampSelection();
		}

		//----------
		void MultipleChoice::setSelection(int selectionIndex) {
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
				this->selectionIndex = -1;
			} else {
				this->selectionIndex = (int) (find - this->options.begin());
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
		void MultipleChoice::entangle(ofParameter<int> & parameter) {
			//if we change, update the parameter
			this->onValueChange += [&parameter](const int & selection) {
				parameter.set(selection);
			};

			//if we're out of sync, update ourselves
			this->onUpdate += [this, &parameter](ofxCvGui::UpdateArguments &) {
				if (parameter.get() != this->getSelectionIndex()) {
					this->setSelection(parameter.get());
				}
			};
		}

		//----------
		void MultipleChoice::clampSelection() {
			if (this->options.empty()) {
				this->selectionIndex = -1;
			}
			else {
				auto clampedSelection = ofClamp(this->selectionIndex, 0, this->options.size() - 1);
				if (clampedSelection != this->selectionIndex) {
					this->setSelection(clampedSelection);
				}
			}
		}

		//----------
		ofRectangle MultipleChoice::getOptionBounds(int optionIndex) const {
			if (this->options.empty()) {
				return this->optionsBounds;
			}

			//get bounds of a single option
			auto optionBounds = this->optionsBounds;
			optionBounds.width /= (float) this->options.size();
			optionBounds.x += optionBounds.width * optionIndex;

			const float borderSize = 2.0f;
			optionBounds.width -= borderSize * 2.0f;
			optionBounds.x += borderSize;
			optionBounds.height -= borderSize * 2.0f;
			optionBounds.y += borderSize;

			return optionBounds;
		}

	}
}