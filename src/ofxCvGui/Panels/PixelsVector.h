#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "ofTypes.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include <vector>
using namespace std;

namespace ofxCvGui {
	namespace Panels {
		class PixelsVector : public Panels::BaseImage {
		public:
			PixelsVector(const vector<ofPixels>& pixels);
			void update();
		protected:
			void drawImage(const DrawArguments& arguments);
		private:
			const vector<ofPixels>& pixels;
			ofPixels currentFrame;
			ofTexture preview;
		};
	}
}