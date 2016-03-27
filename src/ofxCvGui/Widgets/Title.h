#pragma once
#include "../Element.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class Title : public Element {
		public:
			enum Level {
				H1 = 0,
				H2 = 1,
				H3 = 2,
				MaxLevel = H3
			};
			Title(string caption, Level = Level::H1);
		protected:
			const Level level;
		};
	}
}