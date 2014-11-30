#pragma once

#include "../Element.h"
#include "../../../addons/ofxTextInputField/src/ofxTextInputField.h"

namespace ofxCvGui {
	namespace Utils {
		class TextField : public Element {
		public:
			TextField();
			~TextField();

			const string & getText() const;
			void setText(const string &);
			void clearText();

			void setFont(ofTrueTypeFont &);

			void focus();
			void defocus();
			
			ofxTextInputField & getTextInputField();

			ofxLiquidEvent<string> onTextChange;
			ofxLiquidEvent<string> onHitReturn;
		protected:
			void callbackTextChanged(string &);
			void callbackHitReturn(string &);
			ofxTextInputField textInputField;
		};
	}
}