#pragma once
#include "ofxSingleton.h"

#include "ofRectangle.h"
#include "ofColor.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

//#define OFXCVGUI_DISBALE_SCISSOR

namespace ofxCvGui {
	namespace Utils {
#pragma mark Text
		ofRectangle drawText(const std::string& text, float x, float y, bool background = true, float minHeight = 15, float minWidth = 0, bool scissor = false);
		ofRectangle drawText(const std::string& text, const ofRectangle & bounds, bool background = true, bool scissor = false);
		void drawProcessingNotice(std::string message);
		void drawToolTip(const std::string & text, const glm::vec2 & position);

		std::string makeString(char key);

#pragma mark Animation
		ofColor getBeatingSelectionColor();

#pragma mark Scissor
		class ScissorManager : public ofxSingleton::Singleton<ScissorManager> {
		public:
			ScissorManager();

			bool getScissorEnabled() const;
			void setScissorEnabled(bool);

			void pushScissor(const ofRectangle &);
			void popScissor();

			ofRectangle getScissor() const;
		protected:
			void setScissor(const ofRectangle &);
			bool scissorEnabled;
			std::vector<ofRectangle> scissorHistory;
		};

#pragma mark Math
		ofRectangle operator*(const ofRectangle &, const glm::mat4 &);

#pragma mark Colors
		ofColor toColor(const std::string &);
	}
}