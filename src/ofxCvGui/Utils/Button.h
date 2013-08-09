#pragma once
#include "ofxCvGui/Element.h"
#include "ofxCvGui/Utils/LambdaStack.h"

namespace ofxCvGui {
	namespace Utils {
		class Button : public Element {
		public:
			Button();
			~Button();
			LambdaStack<ofVec2f> onHit; ///< returns mouse coords in local normalised
			LambdaStack<DrawArguments> onDrawUp;
			LambdaStack<DrawArguments> onDrawDown;
		protected:
			bool down;
		};
	}
}