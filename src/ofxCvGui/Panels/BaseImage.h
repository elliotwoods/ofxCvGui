#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxLiquidEvent/src/ofxLiquidEvent.h"

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
			enum Zoomed {ZoomOne, ZoomFit};

			ofxLiquidEvent<DrawCroppedArguments> onDrawCropped;
			Zoomed getZoomed() const;
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
            Zoomed zoom;
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