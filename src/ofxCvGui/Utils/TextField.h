#pragma once

#include "../Element.h"
#include "../../../addons/ofxTextInputField/src/ofxTextInputField.h"

namespace ofxCvGui {
	namespace Utils {
		class TextField : public Element {
		public:
			TextField();
			~TextField();

			void setHintText(const string &);

			const string & getText() const;
			void setText(const string &);
			void clearText();

			void setFont(ofTrueTypeFont &);

			void focus();
			void defocus();
			bool isFocused() const;
			
			ofxTextInputField & getTextInputField();

			ofxLiquidEvent<string> onTextChange;
			ofxLiquidEvent<string> onHitReturn;
		protected:
			void callbackTextChanged(string &);
			void callbackHitReturn(string &);
			ofxTextInputField textInputField;
			string hintText;
		};
	}
}