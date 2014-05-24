#pragma once
#include "Toggle.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class Button : public Toggle {
		public:
			class EventArgs {
			};
			OFXCVGUI_MAKE_ELEMENT_HEADER(Button, string caption, const function<void ()> & buttonCallbackFunction) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Button, caption, buttonCallbackFunction);
			}
			Button(string caption);
			Button(string caption, std::function<void ()>);
			virtual ~Button() { }
			ofxLiquidEvent<EventArgs> onHit;
		protected:
			void init(string caption);
			ofParameter<bool> hitValue;
		};
	}
}