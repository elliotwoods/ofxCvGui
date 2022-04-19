#pragma once
#include "Toggle.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class OFXCVGUI_API_ENTRY Button : public Toggle {
		public:
			Button(std::string caption, int hotKey = 0);
			Button(std::string caption, std::function<void()>, int hotKey = 0);
			virtual ~Button() { }
			ofxLiquidEvent<void> onHit;
		protected:
			void init(std::string caption, int hotKey);
			ofParameter<bool> hitValue;
			bool needsToDrop;
		};
	}
}