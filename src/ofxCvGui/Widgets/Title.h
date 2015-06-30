#pragma once
#include "../Element.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class Title : public Element {
		public:
			enum Level {
				H1,
				H2,
				H3
			};
			OFXCVGUI_MAKE_ELEMENT_HEADER(Title, string caption, Level level = Level::H1) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Title, caption, level);
			}
			Title(string caption, Level = Level::H1);
		protected:
			const Level level;
		};
	}
}