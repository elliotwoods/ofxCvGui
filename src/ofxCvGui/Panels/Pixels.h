#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "ofPixels.h"
#include "ofTexture.h"

namespace ofxCvGui {
	namespace Panels {
		class Pixels : public Panels::BaseImage {
		public:
			Pixels(const ofPixels& pixels);
			void update();
			void refresh();
		protected:
			void drawImage(const DrawArguments& arguments);
		private:
			const ofPixels& pixels;
			ofTexture preview;
		};
	}
}