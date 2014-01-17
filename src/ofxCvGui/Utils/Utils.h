#pragma once
#include "ofRectangle.h"

namespace ofxCvGui {
	namespace Utils {
		ofRectangle getScissor();
		void applyScissor(const ofRectangle &);
		void pushScissor(const ofRectangle &);
		void popScissor();
	}
}