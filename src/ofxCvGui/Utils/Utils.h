#pragma once
#include "ofRectangle.h"

//#define OFXCVGUI_DISBALE_SCISSOR

namespace ofxCvGui {
	namespace Utils {
#pragma mark Text
		ofRectangle drawText(const string& text, float x, float y, bool background = true, float minHeight = 15, float minWidth = 0);
		ofRectangle drawText(const string& text, const ofRectangle & bounds, bool background = true);
		void drawProcessingNotice(string message);
		void drawToolTip(const string & text, const ofVec2f & position);

		string makeString(char key);

#pragma mark Scissor
		ofRectangle getScissor();
		void applyScissor(const ofRectangle &);
		void pushScissor(const ofRectangle &);
		void popScissor();

		bool getScissorEnabled();
		bool disableScissor();
		void enableScissor();
	}
}