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
			typedef std::function<bool()> BoolFunction;

			OFXCVGUI_MAKE_ELEMENT_HEADER(Indicator, string caption, StatusFunction statusFunction) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Indicator, caption, statusFunction);
			}
			Indicator(string caption, StatusFunction statusFunction);
		protected:
			void init(string caption);
			const StatusFunction statusFunction;
			static map<Status, ofColor> * colorMap;
		};
	}
}