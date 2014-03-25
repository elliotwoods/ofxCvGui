#pragma once
#include "Toggle.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class Button : public Toggle {
		public:
			class EventArgs {
			};

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