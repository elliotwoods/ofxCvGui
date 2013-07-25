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
            ~PixelsVector();
			void update();
			const ofPixels & getSelection() const;
		protected:
			void drawImage(float width, float height);
            void drawInfo();
            float getImageWidth() const;
            float getImageHeight() const;
		private:
			const vector<ofPixels>& pixels;
			ofTexture preview;
			int selection;
			int lastSelection;
			size_t lastCount;
            bool refreshPerFrame;
		};
	}
}