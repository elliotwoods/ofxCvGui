#pragma once
#include "../ElementGroup.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class OFXCVGUI_API_ENTRY VerticalStack : public ElementGroup {
		public:
			VerticalStack();
		protected:
			bool needsNewWidth = false;
		};
	}
}