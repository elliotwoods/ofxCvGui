#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "ofPixels.h"
#include "ofTexture.h"

namespace ofxCvGui {
	namespace Panels {
		class Image : public Panels::BaseImage {
		public:
			Image(ofImage& image);
            virtual ~Image();
		protected:
			void drawImage(float width, float height);
            void drawInfo(DrawArguments& arguments);
            float getImageWidth() const;
            float getImageHeight() const;
		private:
			ofImage& image;
		};
	}
}