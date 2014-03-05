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
			Title(string caption, Level);
		protected:
			const Level level;
		};
	}
}