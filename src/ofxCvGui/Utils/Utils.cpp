#include "Utils.h"

#include "Constants.h"

#include "ofxAssets.h"

#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofAppGLFWWindow.h"
#include "ofMatrix3x3.h"
#include "ofMatrix4x4.h"

#include <vector>

namespace ofxCvGui {
	namespace Utils {
#pragma mark Text
		//---------
		ofRectangle drawText(const string& text, float x, float y, bool background, float minHeight, float minWidth, bool scissor) {
			auto & font = ofxAssets::font(ofxCvGui::defaultTypeface, 14);
			bool hasFont = font.isLoaded();

			if (scissor) {
				Utils::pushScissor(ofRectangle(x, y, minWidth, minHeight));
			}

			ofPushStyle();
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
					ofDrawRectangle(bounds);
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
					ofDrawRectangle(bounds);
				ofPopStyle();
				ofSetColor(255);
				ofDrawBitmapString(text, x + 10, y + 20);
			}
			ofPopStyle();

			if (scissor) {
				Utils::popScissor();
			}

			return bounds;
		}

		//---------
		ofRectangle drawText(const string & text, const ofRectangle & bounds, bool background, bool scissor) {
			return drawText(text, bounds.x, bounds.y, background, bounds.height, bounds.width, scissor);
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
		void drawToolTip(const string & text, const ofVec2f & position) {
			//draw ignoring scissor
			auto scissorEnabled = disableScissor();

			auto & font = ofxAssets::font(ofxCvGui::defaultTypeface, 14);
			bool hasFont = font.isLoaded();

			if (!hasFont) {
				ofDrawBitmapStringHighlight(text, position);
			} else {
				const auto rawBounds = font.getStringBoundingBox(text, 0, 0);
				const auto halfTextWidth = rawBounds.getWidth() / 2.0f;
				const auto textHeight = rawBounds.getHeight() - rawBounds.y;
				const ofVec2f textDrawAnchor(position.x - (halfTextWidth + rawBounds.x), position.y - (rawBounds.getHeight() + rawBounds.y) - 15);

				ofPath bubble;
				bubble.setStrokeColor(ofColor(0));
				bubble.setStrokeWidth(1.0f);
				bubble.moveTo(position);
				bubble.lineTo(position + ofVec2f(-5, -5));
				bubble.lineTo(position + ofVec2f(-halfTextWidth - 10, -5));
				bubble.lineTo(position + ofVec2f(-halfTextWidth - 10, -5 - textHeight - 10));
				bubble.lineTo(position + ofVec2f(+halfTextWidth + 10, -5 - textHeight - 10));
				bubble.lineTo(position + ofVec2f(+halfTextWidth + 10, -5));
				bubble.lineTo(position + ofVec2f(+5, -5));
				bubble.close();				
				bubble.draw();

				//draw text
				ofPushStyle();
				ofSetColor(0);
				font.drawString(text, floor(textDrawAnchor.x), floor(textDrawAnchor.y));
				ofPopStyle();
			}

			if (scissorEnabled) {
				enableScissor();
			}
		}

		//---------
		string makeString(char key) {
			switch(key) {
			case ' ':
				return "SPACE";
				break;
			case OF_KEY_BACKSPACE:
				return "BACKSPACE";
				break;
			case OF_KEY_RETURN:
				return "RETURN";
				break;
			case OF_KEY_DOWN:
				return "DOWN";
				break;
			case OF_KEY_UP:
				return "UP";
				break;
			case OF_KEY_LEFT:
				return "LEFT";
				break;
			case OF_KEY_RIGHT:
				return "RIGHT";
				break;
			default:
				return string("") + key;
			}
		}

#pragma mark Animation
		//----------
		ofColor getBeatingSelectionColor() {
			return ofColor((sin(ofGetElapsedTimef() * TWO_PI) + 1.0f) * 127, 0, 0);
		}

#pragma mark Scissor
		//----------
		vector<ofRectangle> scissorHistory;
		
		//----------
		ofRectangle getScissor() {
			GLint bounds[4];
			if (getScissorEnabled()) {
				glGetIntegerv(GL_SCISSOR_BOX, bounds);
			} else {
				glGetIntegerv(GL_VIEWPORT, bounds);
			}
			auto currentScissor = ofRectangle(bounds[0], bounds[1], bounds[2], bounds[3]);
			currentScissor.y = ofGetWindowHeight() - currentScissor.y - bounds[3]; // flip coords
			return currentScissor;
		}
			
		//----------
		void applyScissor(const ofRectangle & bounds) {
#ifndef OFXCVGUI_DISBALE_SCISSOR
			int x = (int) bounds.x;
			int y = (int) (ofGetWindowHeight() - (bounds.y + bounds.height));
			int width = (int) bounds.width;
			int height = (int) bounds.height;
			glEnable(GL_SCISSOR_TEST);
			glScissor(x, y, width, height);
#endif
		}

		//----------
		void pushScissor(const ofRectangle & bounds) {
#ifndef OFXCVGUI_DISBALE_SCISSOR
			const auto currentScissor = getScissor();
			auto & debugScissorHistory = scissorHistory; // just so we can debug in debug mode. this gets optimised away
			scissorHistory.push_back(currentScissor);
			if (scissorHistory.empty()) {
				applyScissor(bounds);
			} else {
				applyScissor(bounds.getIntersection(currentScissor));
			}
#endif
		}

		//----------
		void popScissor() {
#ifndef OFXCVGUI_DISBALE_SCISSOR
			if (scissorHistory.empty()) {
				ofLogError("ofxCvGui::popScissor") << "Scissor history is empty";
				return;
			}
			applyScissor(scissorHistory.back());
			scissorHistory.pop_back();
			if(scissorHistory.empty()) {
				glDisable(GL_SCISSOR_TEST);
				glScissor(0, 0, ofGetWidth(), ofGetHeight());
			}
#endif
		}

		//----------
		bool getScissorEnabled() {
			GLboolean scissorEnabled;
			glGetBooleanv(GL_SCISSOR_TEST, &scissorEnabled);
			return scissorEnabled == GL_TRUE;
		}
		
		//----------
		bool disableScissor() {
			if (getScissorEnabled()) {
				glDisable(GL_SCISSOR_TEST);
				return true;
			}
			else {
				return false;
			}
		}

		//----------
		void enableScissor() {
#ifndef OFXCVGUI_DISBALE_SCISSOR
			glEnable(GL_SCISSOR_TEST);
#endif
		}

#pragma mark Math
		//----------
		ofRectangle operator*(const ofRectangle & rectangle, const ofMatrix4x4 & transform) {
			auto topLeft = rectangle.getTopLeft();
			auto scale = ofVec4f(rectangle.getWidth(), rectangle.getHeight(), 0.0f, 0.0f); // w = 0 so no translate
			topLeft = topLeft * transform;
			scale = scale * transform;
			return ofRectangle(topLeft.x, topLeft.y, scale.x, scale.y);
		}
	}
}