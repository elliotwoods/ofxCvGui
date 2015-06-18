#pragma once
#include "../Element.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class Indicator : public Element {
		public:
			enum Status {
				Clear = 0,
				Good = 1,
				Warning,
				Error
			};
			typedef std::function<Status()> StatusFunction;

			// we would get an ambiguous call if we provide a BoolFunction constructor, so we haven't

			OFXCVGUI_MAKE_ELEMENT_HEADER(Indicator, string caption, StatusFunction statusFunction) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Indicator, caption, statusFunction);
			}
			Indicator(string caption, StatusFunction);
		protected:
			void init(string caption);
			const StatusFunction statusFunction;
			static map<Status, ofColor> * colorMap;
		};
	}
}