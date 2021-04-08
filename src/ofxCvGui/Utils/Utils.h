#pragma once
#include "ofxSingleton.h"

#include "ofRectangle.h"
#include "ofColor.h"
#include "ofCamera.h"
#include "ofGraphics.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

//#define OFXCVGUI_DISBALE_SCISSOR

#define OFXCVGUI_TEXT_BACKGROUND 0x46
#define OFXCVGUI_TEXT_SIZE 15

namespace ofxCvGui {
	namespace Utils {
#pragma mark Text
		ofRectangle drawText(const std::string& text, float x, float y, bool background = true, float minHeight = 15, float minWidth = 0, bool scissor = false, const ofColor & backgroundColor = ofColor(OFXCVGUI_TEXT_BACKGROUND));
		ofRectangle drawText(const std::string& text, const ofRectangle & bounds, bool background = true, bool scissor = false);
		void drawProcessingNotice(std::string message);
		void drawToolTip(const std::string & text, const glm::vec2 & position);
		std::string makeString(char key);

		/// <summary>
		/// A deferred system for rendering annotations to 3D views
		/// </summary>
		class AnnotationManager : public ofxSingleton::Singleton<AnnotationManager> {
		public:
			struct TextAnnotation {
				std::string text;
				glm::vec3 position;
				ofColor color;
				glm::mat4x4 worldViewTransform = ofGetCurrentMatrix(ofMatrixMode::OF_MATRIX_MODELVIEW);
			};
			struct DrawAnnotation {
				std::function<void()> drawCall;
				ofRectangle bounds;
				glm::vec3 position;
				ofColor color;
				glm::mat4x4 worldViewTransform = ofGetCurrentMatrix(ofMatrixMode::OF_MATRIX_MODELVIEW);
			};

			AnnotationManager();

			void clear();
			void annotate(const std::string & text, const glm::vec3 & position, const ofColor & = ofColor(40));
			void annotate(const TextAnnotation&);
			void annotate(const DrawAnnotation&);

			void renderAndClearAnnotations(const ofCamera & camera
				, const ofRectangle & viewport);
		protected:
			std::vector<TextAnnotation> textAnnotations;
			std::vector<DrawAnnotation> drawAnnotations;
		};

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