#pragma once
#include "Toggle.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class Button : public Toggle {
		public:
			class EventArgs {
			};
			OFXCVGUI_MAKE_ELEMENT_HEADER(Button, string caption, const function<void ()> & buttonCallbackFunction, char hotKey = 0) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Button, caption, buttonCallbackFunction, hotKey);
			}
			Button(string caption, char hotKey = 0);
			Button(string caption, std::function<void ()>, char hotKey = 0);
			virtual ~Button() { }
			ofxLiquidEvent<EventArgs> onHit;
		protected:
			void init(string caption, char hotKey);
			ofParameter<bool> hitValue;
			bool needsToDrop;
		};
	}
}