#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "ofPixels.h"
#include "ofTexture.h"

namespace ofxCvGui {
	namespace Panels {
		class Pixels : public Panels::BaseImage {
		public:
			Pixels(const ofPixels& pixels);
            ~Pixels();
			void updateImage();
		protected:
			void drawImage(float width, float height);
            void drawInfo(DrawArguments& arguments);
            float getImageWidth() const;
            float getImageHeight() const;
		private:
			const ofPixels& pixels;
			ofTexture preview;
            bool refreshPerFrame;
		};

		shared_ptr<Pixels> makePixels(const ofPixels& asset, string caption = "");
	}
}