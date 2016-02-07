#pragma once
#include "../Element.h"
#include "ofParameter.h"

#include "ofxCvGui/Utils/TextField.h"

namespace ofxCvGui {
	namespace Widgets {
		class TextField : public Element {
		public:
			OFXCVGUI_MAKE_ELEMENT_HEADER(TextField, string caption) {
				OFXCVGUI_MAKE_ELEMENT_BODY(TextField, caption);
			}
			
			TextField(string caption);
			TextField(ofParameter<string> &);
			
			shared_ptr<ofxCvGui::Utils::TextField> getTextField();
		protected:
			void parameterCallback(string &);
			
			shared_ptr<ofxCvGui::Utils::TextField> textField;
			
			ofParameter<string> * parameter = 0;
			bool disableTextFieldEvents = false;
		};
		
		shared_ptr<TextField> make(ofParameter<string> &);
	}
}