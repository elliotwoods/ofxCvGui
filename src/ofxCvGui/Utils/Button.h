#pragma once
#include "../Element.h"

namespace ofxCvGui {
	namespace Utils {
		class Button : public Element {
		public:
			Button();
			~Button();
			bool isDown() const;

			ofxLiquidEvent<MouseArguments> onButtonHit; ///< returns mouse coords in local normalised
			ofxLiquidEvent<DrawArguments> onDrawUp;
			ofxLiquidEvent<DrawArguments> onDrawDown;
		protected:
			bool down;
		};
	}
}