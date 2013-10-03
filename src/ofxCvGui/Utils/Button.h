#pragma once
#include "ofxCvGui/Element.h"
#include "ofxLiquidEvent.h"

namespace ofxCvGui {
	namespace Utils {
		class Button : public Element {
		public:
			Button();
			~Button();
			ofxLiquidEvent<ofVec2f> onHit; ///< returns mouse coords in local normalised
			ofxLiquidEvent<DrawArguments> onDrawUp;
			ofxLiquidEvent<DrawArguments> onDrawDown;
		protected:
			bool down;
		};
	}
}