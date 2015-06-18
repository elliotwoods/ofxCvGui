#pragma once
#include "ofRectangle.h"
#include "ofMatrix4x4.h"

//#define OFXCVGUI_DISBALE_SCISSOR

namespace ofxCvGui {
	namespace Utils {
#pragma mark Text
		ofRectangle drawText(const string& text, float x, float y, bool background = true, float minHeight = 15, float minWidth = 0, bool scissor = false);
		ofRectangle drawText(const string& text, const ofRectangle & bounds, bool background = true, bool scissor = false);
		void drawProcessingNotice(string message);
		void drawToolTip(const string & text, const ofVec2f & position);

		string makeString(char key);

#pragma mark Animation
		ofColor getBeatingSelectionColor();

#pragma mark Scissor
		ofRectangle getScissor();
		void applyScissor(const ofRectangle &);
		void pushScissor(const ofRectangle &);
		void popScissor();

		bool getScissorEnabled();
		bool disableScissor();
		void enableScissor();

#pragma mark Math
		ofRectangle operator*(const ofRectangle &, const ofMatrix4x4 &);
	}
}