#pragma once
#include "Toggle.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class Button : public Toggle {
		public:
			Button(string caption, char hotKey = 0);
			Button(string caption, std::function<void ()>, char hotKey = 0);
			virtual ~Button() { }
			ofxLiquidEvent<void> onHit;
		protected:
			void init(string caption, char hotKey);
			ofParameter<bool> hitValue;
			bool needsToDrop;
		};
	}
}