#pragma once
#include "../Element.h"
#include "ofParameter.h"

#include "ofxCvGui/Utils/TextField.h"

namespace ofxCvGui {
	namespace Widgets {
		class TextField : public Element {
		public:
			TextField(string caption);
			TextField(ofParameter<string> &);
			
			shared_ptr<ofxCvGui::Utils::TextField> getTextField();
		protected:
			void parameterCallback(string &);
			
			shared_ptr<ofxCvGui::Utils::TextField> textField;
			
			ofParameter<string> * parameter = 0;
			bool disableTextFieldEvents = false;
		};
	}
}