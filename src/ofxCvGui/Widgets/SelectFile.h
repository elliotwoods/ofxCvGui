#pragma once

#include "EditableValue.h"

#include "ofParameter.h"
#include "ofSystemUtils.h"

namespace ofxCvGui {
	namespace Widgets {
		class SelectFile : public LiveValue<string> {
		public:
			OFXCVGUI_MAKE_ELEMENT_HEADER(SelectFile, string caption, function<string()> get, function<void(string &)> set) {
				OFXCVGUI_MAKE_ELEMENT_BODY(SelectFile, caption, get, set);
			}
			OFXCVGUI_MAKE_ELEMENT_HEADER(SelectFile, ofParameter<string> & filename) {
				OFXCVGUI_MAKE_ELEMENT_BODY(SelectFile, filename);
			}
			SelectFile(string caption, function<string()> get, function<void(string &)> set);
			SelectFile(ofParameter<string> &);
		protected:
			function<void(string &)> setFunction;
		};
	}
}