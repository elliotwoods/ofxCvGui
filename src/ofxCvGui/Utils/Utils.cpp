#include "pch_ofxCvGui.h"

OFXSINGLETON_DEFINE(ofxCvGui::Utils::AnnotationManager);
OFXSINGLETON_DEFINE(ofxCvGui::Utils::ScissorManager);


#include <GLFW/glfw3.h>

using namespace std;

namespace ofxCvGui {
	namespace Utils {

#pragma mark Text
		//---------
		ofRectangle drawText(const string& text
			, float x, float y
			, bool background
			, float minHeight, float minWidth
			, bool scissor
			, const ofColor backgroundColor
			, string typeface
			, bool dontActuallyDrawAnything) {

			auto & font = ofxAssets::font(typeface, OFXCVGUI_TEXT_SIZE);
			bool hasFont = font.isLoaded();

			if (scissor) {
				Utils::ScissorManager::X().pushScissor(ofRectangle(x, y, minWidth, minHeight));
			}

			ofRectangle bounds(x, y, 0, 0);

			ofPushStyle();
			{
				ofPushStyle();
				ofSetColor(backgroundColor);
				ofFill();
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
					if (background) {
						if (!dontActuallyDrawAnything) {
							ofDrawRectangle(bounds);
						}
					}
					ofPopStyle();
					if (backgroundColor.getBrightness() > 150) {
						ofSetColor(0);
					}
					else {
						ofSetColor(255);
					}
					x = x + font.getSize() / 2;
					if (!multiline) {
						y = y + (bounds.height + rawHeight * 2.0f / 3.0f) / 2.0f;
					}
					else {
						y = y + font.getLineHeight();
					}
					if (!dontActuallyDrawAnything) {
						font.drawString(text, (int)x, (int)y);
					}
				}
				else {
					bounds = ofRectangle(x, y, text.length() * 10 + 20, 30);
					if (background) {
						if (!dontActuallyDrawAnything) {
							ofDrawRectangle(bounds);
						}
					}
					ofPopStyle();
					if (backgroundColor.getBrightness() > 200) {
						ofSetColor(0);
					}
					else {
						ofSetColor(255);
					}
					if (!dontActuallyDrawAnything) {
						ofDrawBitmapString(text, x + 10, y + 20);
					}
				}
			}
			ofPopStyle();

			if (scissor) {
				Utils::ScissorManager::X().popScissor();
			}

			return bounds;
		}

		//---------
		ofRectangle drawText(const string & text, const ofRectangle & bounds, bool background, bool scissor) {
			return drawText(text, bounds.x, bounds.y, background, bounds.height, bounds.width, scissor);
		}

		//---------
		ofRectangle drawGlyph(const string& glyph, const ofRectangle& bounds) {
			auto glyphTypeface = ofxCvGui::getGlyphTypeface();
			
			// ensure glyphs are loaded
			{
				auto fonts = ofxAssets::Register::X().getFonts();
				auto findFont = fonts.find(glyphTypeface);
				if (findFont != fonts.end()) {
					findFont->second->setFullCharacterSetEnabled(true);
				}
			}

			auto& font = ofxAssets::font(glyphTypeface, OFXCVGUI_TEXT_SIZE);

			// calculate position
			auto priorBounds = font.getStringBoundingBox(glyph, bounds.x, bounds.y);
			auto offset = bounds.getCenter() - priorBounds.getCenter();

			// draw
			font.drawString(glyph, bounds.x + offset.x, bounds.y + offset.y);

			// calculate corrected bounds for return
			auto correctedBounds = priorBounds;
			correctedBounds.x += offset.x;
			correctedBounds.y += offset.y;
			return correctedBounds;
		}

		//---------
		void drawProcessingNotice(string message) {
			auto window = ofGetWindowPtr();
			auto glfwAppWindow = dynamic_cast<ofAppGLFWWindow*>(window);
			if (glfwAppWindow) {
				auto glfwWindow = glfwAppWindow->getGLFWWindow();
				ofClear(0, 0);
				drawText(message, 0, 0, true, ofGetHeight(), ofGetWidth());
				glfwMakeContextCurrent(glfwWindow);
				glfwSwapBuffers(glfwWindow);
				glfwPollEvents();
			}
		}

		//---------
		void drawToolTip(const string & text, const glm::vec2 & position) {
			//draw ignoring scissor
			auto scissorEnabled = ScissorManager::X().getScissorEnabled();
			ScissorManager::X().setScissorEnabled(false);

			auto & font = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 14);
			bool hasFont = font.isLoaded();

			if (!hasFont) {
				ofDrawBitmapStringHighlight(text, position);
			} else {
				const auto rawBounds = font.getStringBoundingBox(text, 0, 0);
				const auto halfTextWidth = rawBounds.getWidth() / 2.0f;
				const auto textHeight = rawBounds.getHeight() - rawBounds.y;
				const glm::vec2 textDrawAnchor(position.x - (halfTextWidth + rawBounds.x), position.y - (rawBounds.getHeight() + rawBounds.y) - 15);

				ofPath bubble;
				bubble.setStrokeColor(ofColor(0));
				bubble.setStrokeWidth(1.0f);
				bubble.moveTo(position);
				bubble.lineTo(position + glm::vec2(-5, -5));
				bubble.lineTo(position + glm::vec2(-halfTextWidth - 10, -5));
				bubble.lineTo(position + glm::vec2(-halfTextWidth - 10, -5 - textHeight - 10));
				bubble.lineTo(position + glm::vec2(+halfTextWidth + 10, -5 - textHeight - 10));
				bubble.lineTo(position + glm::vec2(+halfTextWidth + 10, -5));
				bubble.lineTo(position + glm::vec2(+5, -5));
				bubble.close();				
				bubble.draw();

				//draw text
				ofPushStyle();
				ofSetColor(0);
				font.drawString(text, floor(textDrawAnchor.x), floor(textDrawAnchor.y));
				ofPopStyle();
			}

			if (scissorEnabled) {
				ScissorManager::X().setScissorEnabled(true);
			}
		}

		//---------
		string makeString(int key) {
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
				return string("") + char(key);
			}
		}

		//---------
		void drawTextAnnotation(const std::string& text, const glm::vec3& position, const ofColor& color) {
			AnnotationManager::X().annotate(text, position, color);
		}

		//---------
		void drawGraphicAnnotation(const std::function<void()>& drawCall
			, const ofRectangle& bounds
			, const glm::vec3& position
			, const ofColor& color) {
			AnnotationManager::X().annotate(Utils::AnnotationManager::DrawAnnotation{
				[bounds, drawCall, color]() {
					// Draw background to graphic
					ofPushStyle();
					{
						ofFill();
						ofSetColor(color);
						ofDrawRectangle(bounds);
					}
					ofPopStyle();
					drawCall();
				}
				, bounds
				, position
				, color
				});
		}

		//----------
		AnnotationManager::AnnotationManager() {

		}

		//----------
		void AnnotationManager::clear() {
			this->textAnnotations.clear();
			this->drawAnnotations.clear();
		}

		//----------
		void AnnotationManager::annotate(const std::string& text, const glm::vec3& position, const ofColor & color) {
			if (!this->enabled) {
				return;
			}
			this->annotate(TextAnnotation {
				text
				, position
				, color
				});
		}
		//----------
		void AnnotationManager::annotate(const TextAnnotation& textAnnotation) {
			if (!this->enabled) {
				return;
			}
			this->textAnnotations.push_back(textAnnotation);
		}

		//----------
		void AnnotationManager::annotate(const DrawAnnotation& drawAnnotation) {
			if (!this->enabled) {
				return;
			}
			this->drawAnnotations.push_back(drawAnnotation);
		}

		//----------
		void AnnotationManager::renderAndClearAnnotations(const ofCamera & camera
			, const ofRectangle & viewport) {
			const float stepSize = 10;

			// Convert the text annotations into draw annotations
			for (const auto& textAnnotation : this->textAnnotations) {
				this->annotate(DrawAnnotation {
					[textAnnotation]() {
						drawText(textAnnotation.text, 0, 0, true, 15, 0, false, textAnnotation.color);
					}
					, drawText(textAnnotation.text
						, 0
						, 0
						, true
						, 15
						, 0
						, false
						, ofColor(0)
						, ofxCvGui::getDefaultTypeface()
						, true) // get the bounds by calling the draw command now
					, textAnnotation.position
					, textAnnotation.color
					, textAnnotation.worldViewTransform
					});
			}

			// Store existing drawn animations
			std::vector<ofRectangle> existingAnnotationBounds; // don't draw over existing annotations

			// Function to try and draw
			auto tryDrawAnnotation = [&](const DrawAnnotation& drawAnnotation, float x, float y, glm::vec3 & target, bool onLeftSide) {
				
				// Get bounds for what we'll draw
				auto bounds = drawAnnotation.bounds;
				if (onLeftSide) {
					x -= bounds.width;
				}
				y -= bounds.height / 2;
				bounds.x += x;
				bounds.y += y;

				// Check bounds doesn't overlap other rectangles
				{
					for (const auto& existing : existingAnnotationBounds)
					{
						if (!bounds.getIntersection(existing).isEmpty()) {
							// There is an intersection
							return false;
						}
					}
				}

				// Draw the annotation
				{
					ofPushMatrix();
					{
						ofTranslate(x, y);
						drawAnnotation.drawCall();
					}
					ofPopMatrix();
				}

				// Draw the underline and point to target
				{
					ofPushStyle();
					{
						ofSetColor(drawAnnotation.color);

						if (onLeftSide) {
							ofDrawLine(glm::vec2(bounds.getRight(), bounds.y + bounds.height / 2), target);
						}
						else {
							ofDrawLine(glm::vec2(bounds.getLeft(), bounds.y + bounds.height / 2), target);
						}
					}
					ofPopStyle();
					
				}

				existingAnnotationBounds.push_back(bounds);

				return true;
			};

			auto cameraViewInverse = glm::inverse(camera.getModelViewMatrix());

			// Draw the annotations
			for (const auto& drawAnnotation : this->drawAnnotations) {
				glm::vec4 objectSpace{
					drawAnnotation.position.x
					, drawAnnotation.position.y
					, drawAnnotation.position.z
					, 1.0f
				};
				auto worldSpace = cameraViewInverse * drawAnnotation.worldViewTransform * objectSpace;
				worldSpace /= worldSpace.w;
				auto screenPosition = camera.worldToScreen(worldSpace, viewport);

				// Ignore if outside of screen
				if (!viewport.inside(screenPosition) 
					|| screenPosition.z < -1.0f
					|| screenPosition.z > 1.0f) {
					continue;
				}

				// move to local coordinates after testing if inside viewport
				{
					screenPosition.x -= viewport.x;
					screenPosition.y -= viewport.y;
				}

				bool drawn = false;
				float x;

				// Try to draw on the right
				x = screenPosition.x + stepSize;
				if (x < viewport.getRight()) {
					// Try to draw to the right below
					{
						for (float y = screenPosition.y; y < viewport.getBottom(); y += stepSize) {
							if (tryDrawAnnotation(drawAnnotation, x, y, screenPosition, false)) {
								drawn = true;
								break;
							}
						}
						if (drawn) {
							continue;
						}
					}

					// Try to draw to the right above
					{
						for (float y = screenPosition.y - stepSize; y < viewport.getBottom(); y -= stepSize) {
							if (tryDrawAnnotation(drawAnnotation, x, y, screenPosition, false)) {
								drawn = true;
								break;
							}
						}
						if (drawn) {
							continue;
						}
					}
				}

				// Try to draw on the left
				x = screenPosition.x - stepSize;
				if (x > viewport.getLeft()) {
					// Try to draw to the right below
					{
						for (float y = screenPosition.y; y < viewport.getBottom(); y += stepSize) {
							if (tryDrawAnnotation(drawAnnotation, x, y, screenPosition, true)) {
								drawn = true;
								break;
							}
						}
						if (drawn) {
							continue;
						}
					}

					// Try to draw to the right above
					{
						for (float y = screenPosition.y - stepSize; y < viewport.getBottom(); y -= stepSize) {
							if (tryDrawAnnotation(drawAnnotation, x, y, screenPosition, true)) {
								drawn = true;
								break;
							}
						}
						if (drawn) {
							continue;
						}
					}
				}
			}

			// Clear the annotations
			this->clear();
		}

		//----------
		void
			AnnotationManager::setEnabled(bool enabled)
		{
			this->enabled = enabled;
		}

#pragma mark Animation
		//----------
		ofColor getBeatingSelectionColor() {
			return ofColor((sin(ofGetElapsedTimef() * TWO_PI) + 1.0f) * 127, 0, 0);
		}

#pragma mark ScissorManager
		// NOTE : ScissorManager caches the scissor state to reduce calls to glGet (can cause up to a 30% overhead on app performance)
		//----------
		ScissorManager::ScissorManager() {
			this->scissorEnabled = false;
		}

		//----------
		bool ScissorManager::getScissorEnabled() const {
			return this->scissorEnabled;
		}

		//----------
		void ScissorManager::setScissorEnabled(bool scissorEnabled) {
			if (scissorEnabled != this->scissorEnabled) {
				if (scissorEnabled) {
#ifndef OFXCVGUI_DISABLE_SCISSOR
					glEnable(GL_SCISSOR_TEST);
#endif
					this->scissorEnabled = true;
				}
				else {
#ifndef OFXCVGUI_DISABLE_SCISSOR
					glDisable(GL_SCISSOR_TEST);
#endif
					this->scissorEnabled = false;
				}
			}
		}
		
		//----------
		void ScissorManager::pushScissor(const ofRectangle & scissor) {
			//the last one the stack is the current one

			auto currentScissor = this->getScissor();
			auto intersectScissor = scissor.getIntersection(currentScissor);

			this->scissorHistory.push_back(intersectScissor);
			this->setScissor(intersectScissor);
		}

		//----------
		void ScissorManager::popScissor() {
			//the last one the stack is the current one

			if (scissorHistory.empty()) {
				ofLogError("ofxCvGui::popScissor") << "Scissor history is empty";
				return;
			}

			scissorHistory.pop_back();

			if (scissorHistory.empty()) {
				this->setScissorEnabled(false);
			} else {
				this->setScissor(scissorHistory.back());
			}
		}

		//----------
		ofRectangle ScissorManager::getScissor() const {
			if (this->getScissorEnabled() && !this->scissorHistory.empty()) {
				return this->scissorHistory.back();
			}
			else {
				return ofRectangle(0.0f, 0.0f, ofGetWidth(), ofGetHeight());
			}
		}
		//----------
		void ScissorManager::setScissor(const ofRectangle & scissor) {
			this->setScissorEnabled(true);
#ifndef OFXCVGUI_DISABLE_SCISSOR
			int x = (int)scissor.x;
			int y = (int)(ofGetHeight() - (scissor.y + scissor.height));
			int width = (int)scissor.width;
			int height = (int)scissor.height;
			glScissor(x, y, width, height);
#endif
		}

#pragma mark Math
		//----------
		ofRectangle operator*(const ofRectangle & rectangle, const glm::mat4x4 & transform) {
			auto topLeft = rectangle.getTopLeft();
			auto scale = glm::vec4(rectangle.getWidth()
				, rectangle.getHeight()
				, 0.0f
				, 0.0f); // w = 0 so no translate
			topLeft = transform * glm::vec4(topLeft, 1);
			scale = transform * scale;
			return ofRectangle(topLeft.x, topLeft.y, scale.x, scale.y);
		}

#pragma mark Color
		//----------
		ofColor toColor(const string & text) {
			static map<string, ofColor> colorMap;
			auto findColor = colorMap.find(text);
			if (findColor == colorMap.end()) {
				auto hash = std::hash<string>()(text);
				auto hue = hash % 256;
				auto saturation = (hash >> 8) % 128;
				auto brightness = (hash >> 12) % 64 + 192;

				ofColor color;
				color.setHsb(hue, saturation, brightness);
				colorMap.insert(make_pair(text, color));
				return color;
			}
			else {
				return findColor->second;
			}
		}
	}
}