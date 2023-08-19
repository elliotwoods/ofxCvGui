#pragma once
#include "../ElementGroup.h"
#include <string>

#include "ofxCvGui/Widgets/Button.h"

namespace ofxCvGui {
	namespace Widgets {
		class OFXCVGUI_API_ENTRY HorizontalStack : public ElementGroup {
		public:
			HorizontalStack();
			shared_ptr<Widgets::Button> addButton(const string&, const std::function<void()>&);
		protected:
			bool needsNewHeight = false;
		};
	}
}