#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		MultipleChoice::MultipleChoice(const string & caption) {
			this->setCaption(caption);

			this->setBounds(ofRectangle(0, 0, 100, 60));
			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				//draw caption
				auto & captionFont = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 12);
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
		MultipleChoice::MultipleChoice(const string & caption, const initializer_list<string> & options) :
		MultipleChoice(caption) {
			for (const auto & option : options) {
				this->addOption(option);
			}
		}

		//----------
		void MultipleChoice::addOption(string choice) {
			this->options.push_back(choice);
			this->clampSelection(); // also moves up the selection to 0 if we have allowNullSelection to false 
		}


		//----------
		void MultipleChoice::addOptions(initializer_list<string> options) {
			for (const auto & option : options) {
				this->addOption(option);
			}
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