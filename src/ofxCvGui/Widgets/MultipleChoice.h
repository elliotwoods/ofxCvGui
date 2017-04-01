#pragma once
#include "Toggle.h"

namespace ofxCvGui {
	namespace Widgets {
		class MultipleChoice : public Element {
		public:
			MultipleChoice(const string & caption);
			MultipleChoice(const string & caption, const initializer_list<string> & options);
			void addOption(string);
			void addOptions(initializer_list<string>);
			void removeOption(string);
			void clearOptions();

			void setSelection(int);
			void setSelection(string);

			int getSelectionIndex() const;
			string getSelection() const;

			void setAllowNullSelection(bool);
			bool getAllowNullSelection() const;

			//entangle this multiple choice with an ofParameter<enum>
			template<typename EnumType>
			void entangle(ofParameter<EnumType> & parameter) {
				//if we change, update the parameter
				this->onValueChange += [&parameter](const int & selection) {
					parameter.set((EnumType)selection);
				};

				//if we're out of sync, update ourselves
				this->onUpdate += [this, &parameter](ofxCvGui::UpdateArguments &) {
					if ((int) parameter.get() != this->getSelectionIndex()) {
						this->setSelection((int) parameter.get());
					}
				};
			}

			ofxLiquidEvent<const int> onValueChange;
		protected:
			void clampSelection();
			ofRectangle getOptionBounds(int optionIndex) const;

			vector<string> options;
			int selectionIndex = -1;
			bool allowNullSelection = false;
			ofRectangle optionsBounds;
		};
	}
}