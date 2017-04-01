#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxLiquidEvent.h"

namespace ofxCvGui {
	enum ImageZoomState { ZoomX1, ZoomX10, Stretch, Fit };

	struct DrawImageArguments {
		DrawArguments drawArguments;
		ImageZoomState zoomState;
		ofRectangle drawBounds;
	};
	
	namespace Panels {
		class BaseImage : public Base {
		public:

			ofxLiquidEvent<DrawImageArguments> onDrawImage;
			ImageZoomState getImageZoomState() const;

			void setMirror(bool);
			bool getMirror() const;

			ofMatrix4x4 getPanelToImageTransform() const;

			float getZoomFactor() const;
		protected:
            BaseImage();
            virtual ~BaseImage();
			virtual void drawImage(float width, float height) = 0;
            virtual float getImageWidth() const = 0;
            virtual float getImageHeight() const = 0;
            void nudgeZoom(KeyboardArguments &);
			void clampScroll();

            ofVec2f scroll;
			ImageZoomState zoomState = ImageZoomState::Stretch;
			bool mirror = false;
		};
	}
}