#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxLiquidEvent.h"

namespace ofxCvGui {
	
	struct DrawImageArguments {
		DrawImageArguments(bool zoomed, const ofVec2f & drawSize, const ofVec2f & viewSize, const ofVec2f & offsetCropped);
		bool zoomed;
		ofVec2f drawSize;
		ofVec2f viewSize;
		ofVec2f offsetCropped;
	};
	
	namespace Panels {
		class BaseImage : public Base {
		public:
			enum Zoomed {ZoomOne, ZoomFit};

			ofxLiquidEvent<DrawImageArguments> onDrawImage;
			Zoomed getZoomed() const;

			void setMirror(bool);
			bool getMirror() const;

			ofMatrix4x4 getPanelToImageTransform() const;
		protected:
            BaseImage();
            virtual ~BaseImage();
			void drawImage(DrawArguments& arguments);
			virtual void drawImage(float width, float height) = 0;
            virtual float getImageWidth() const = 0;
            virtual float getImageHeight() const = 0;
            void nudgeZoom(KeyboardArguments &);
			void applyMirror() const;
            ofVec2f scroll;
            Zoomed zoom;
			bool mirror = false;
            ofVec2f getScrollClamped() const;
		};
	}
}