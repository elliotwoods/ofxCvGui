#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Assets.h"

namespace ofxCvGui {
	namespace Panels {
		class BaseImage : public Base {
		public:
			struct DrawCroppedArguments {
				DrawCroppedArguments(bool zoomed, const ofVec2f & size, const ofVec2f & offsetCropped);
				bool zoomed;
				ofVec2f size;
				ofVec2f offsetCropped;
			};
			ofxCvGui::Utils::LambdaStack<DrawCroppedArguments> onDrawCropped;
		protected:
            BaseImage();
            virtual ~BaseImage();
			void drawImage(DrawArguments& arguments);
			virtual void drawImage(float width, float height) = 0;
            virtual float getImageWidth() const = 0;
            virtual float getImageHeight() const = 0;
            void mouseAction(MouseArguments &);
            void nudgeZoom(KeyboardArguments &);
            static ofVec2f scroll;
            enum {ZoomOne, ZoomFit} zoom;
            ofVec2f getScrollClamped() const;
        private:
            ofRectangle buttonFitBounds;
            ofRectangle buttonOneBounds;
            ofRectangle zoomBox;
            ofRectangle zoomSelection;
            enum {DragNone, DragImage, DragZoomBox} dragSelection;
		};
	}
}