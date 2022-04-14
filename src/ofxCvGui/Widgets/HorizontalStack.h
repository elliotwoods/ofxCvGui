#pragma once
#include "../ElementGroup.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class OFXCVGUI_API_ENTRY HorizontalStack : public ElementGroup {
		public:
			HorizontalStack();
		protected:
			bool needsNewHeight = false;
		};
	}
}