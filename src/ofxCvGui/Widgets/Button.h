#pragma once
#include "Toggle.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class OFXCVGUI_API_ENTRY Button : public Toggle {
		public:
			Button(std::string caption, char hotKey = 0);
			Button(std::string caption, std::function<void ()>, char hotKey = 0);
			virtual ~Button() { }
			ofxLiquidEvent<void> onHit;
		protected:
			void init(std::string caption, char hotKey);
			ofParameter<bool> hitValue;
			bool needsToDrop;
		};
	}
}