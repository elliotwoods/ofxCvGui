#pragma once
#include "Toggle.h"

namespace ofxCvGui {
	namespace Widgets {
		class OFXCVGUI_API_ENTRY MultipleChoice : public Element {
		public:
			MultipleChoice(const string& caption);
			MultipleChoice(const string& caption, const initializer_list<string>& options);
			MultipleChoice(const string& caption, const vector<string> & options);
			void addOption(string);
			void addOptions(vector<string>);
			void removeOption(string);
			void clearOptions();

			void setSelection(int);
			void setSelection(string);

			int getSelectionIndex() const;
			string getSelection() const;

			void setAllowNullSelection(bool);
			bool getAllowNullSelection() const;

			void setGlyphs(const vector<string>&);

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
			void rebuildOptions();

			vector<string> options;
			vector<string> glyphs;
			shared_ptr<Element> optionsElement = make_shared<Element>();

			int selectionIndex = -1;
			bool allowNullSelection = false;

			static const float radius;
		};
	}
}