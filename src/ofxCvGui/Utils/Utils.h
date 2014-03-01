#pragma once
#include "ofRectangle.h"

namespace ofxCvGui {
	namespace Utils {
#pragma mark Text
		ofRectangle drawText(const string& text, float x, float y, bool background = true, float minHeight = 15, float minWidth = 0);

#pragma mark Scissor
		ofRectangle getScissor();
		void applyScissor(const ofRectangle &);
		void pushScissor(const ofRectangle &);
		void popScissor();
	}
}