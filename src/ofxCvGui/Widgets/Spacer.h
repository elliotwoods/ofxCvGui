#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Spacer : public Element {
		public:
			OFXCVGUI_MAKE_ELEMENT_HEADER(Spacer) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Spacer);
			}
			Spacer();
		};
	}
}