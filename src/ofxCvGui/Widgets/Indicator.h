#pragma once
#include "../Element.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class Indicator : public Element {
		public:
			enum Status {
				Clear,
				Good,
				Warning,
				Error
			};
			typedef std::function<Status()> Function;

			OFXCVGUI_MAKE_ELEMENT_HEADER(Indicator, string caption, Function statusFunction) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Indicator, caption, statusFunction);
			}
			Indicator(string caption, Function statusFunction);
		protected:
			const Function statusFunction;
			static map<Status, ofColor> * colorMap;
		};
	}
}