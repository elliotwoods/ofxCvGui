#include "Utils.h"
#include "Constants.h"
#include "ofxAssets.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofAppGLFWWindow.h"

#include <vector>

namespace ofxCvGui {
	namespace Utils {
#pragma mark Text
		//---------
		ofRectangle drawText(const string& text, float x, float y, bool background, float minHeight, float minWidth) {
			auto & font = ofxAssets::AssetRegister.getFont(ofxCvGui::defaultTypeface, 14);
			bool hasFont = font.isLoaded();

			ofPushStyle();
			ofSetColor(0x46);
			ofFill();
			ofRectangle bounds(x, y, 0, 0);
			bool multiline = ofIsStringInString(text, "\n");

			if (hasFont) {
				ofRectangle rawBounds = font.getStringBoundingBox(text, x, y);
				float rawWidth = rawBounds.width;
				float rawHeight;
				if (!multiline)
					rawHeight = font.getStringBoundingBox("Hy", x, y).height;
				else
					rawHeight = rawBounds.height + font.getLineHeight() * 0.5f;
				bounds.x = x;
				bounds.y = y;
				bounds.width = rawWidth + font.getSize();
				if (bounds.width < minWidth) {
					x += (minWidth - bounds.width) / 2.0f;
					bounds.width = minWidth;
				}
				bounds.height = MAX(rawHeight, minHeight);
				if (background)
					ofRect(bounds);
				ofPopStyle();
				ofSetColor(255);
				x = x + font.getSize() / 2;
				if (!multiline)
					y = y + (bounds.height + rawHeight * 2.0f / 3.0f) / 2.0f;
				else
					y = y + font.getLineHeight();
				font.drawString(text, (int) x, (int) y);
			} else {
				bounds = ofRectangle(x, y, text.length() * 10 + 20, 30);
				if (background)
					ofRect(bounds);
				ofPopStyle();
				ofSetColor(255);
				ofDrawBitmapString(text, x + 10, y + 20);
			}

			return bounds;
		}

		//---------
		ofRectangle drawText(const string & text, const ofRectangle & bounds, bool background) {
			return drawText(text, bounds.x, bounds.y, background, bounds.height, bounds.width);
		}

		//---------
		void drawProcessingNotice(string message) {
			auto window = glfwGetCurrentContext();
			if (window) {
				ofClear(0, 0);
				drawText(message, 0, 0, true, ofGetHeight(), ofGetWidth());
				glfwSwapBuffers(window);
				glFlush();
			}
		}

		//---------
		string makeString(char key) {
			switch(key) {
			case ' ':
				return "SPACE";
			case OF_KEY_BACKSPACE:
				return "BACKSPACE";
			case OF_KEY_RETURN:
				return "RETURN";
			case OF_KEY_DOWN:
				return "DOWN";
			case OF_KEY_UP:
				return "UP";
			case OF_KEY_LEFT:
				return "LEFT";
			case OF_KEY_RIGHT:
				return "RIGHT";
			default:
				return string("") + key;
			}
		}

#pragma mark Scissor
		//----------
		vector<ofRectangle> scissorHistory;
		
		//----------
		ofRectangle getScissor() {
			int bounds[4];
			glGetIntegeri_v(GL_SCISSOR_BOX, 0, bounds);
			return ofRectangle(bounds[0], bounds[1], bounds[2], bounds[3]);
		}
			
		//----------
		void applyScissor(const ofRectangle & bounds) {
			int x = (int) bounds.x;
			int y = (int) (ofGetWindowHeight() - (bounds.y + bounds.height));
			int width = (int) bounds.width;
			int height = (int) bounds.height;
			glEnable(GL_SCISSOR_TEST);
			glScissor(x, y, width, height);
		}

		//----------
		void pushScissor(const ofRectangle & bounds) {
			scissorHistory.push_back(getScissor());
			if (scissorHistory.empty()) {
				applyScissor(bounds);
			} else {
				applyScissor(bounds.getIntersection(scissorHistory.back()));
			}
		}

		//----------
		void popScissor() {
			applyScissor(scissorHistory.back());
			scissorHistory.pop_back();
			if(scissorHistory.empty()) {
				glDisable(GL_SCISSOR_TEST);
			}
		}
	}
}