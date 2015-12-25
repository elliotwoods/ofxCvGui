#pragma once
#include "Toggle.h"

namespace ofxCvGui {
	namespace Widgets {
		class MultipleChoice : public Element {
		public:
			OFXCVGUI_MAKE_ELEMENT_HEADER(MultipleChoice, string caption) {
				OFXCVGUI_MAKE_ELEMENT_BODY(MultipleChoice, caption);
			}
			MultipleChoice(string caption);
			void addOption(string);
			void removeOption(string);
			void clearOptions();

			void setSelection(int);
			void setSelection(string);

			int getSelectionIndex() const;
			string getSelection() const;

			void entangle(ofParameter<int> &); ///< Keep a parameter synchronised

			ofxLiquidEvent<const int> onValueChange;
		protected:
			void clampSelection();
			ofRectangle getOptionBounds(int optionIndex) const;

			vector<string> options;
			int selectionIndex;
			ofRectangle optionsBounds;
		};
	}
}