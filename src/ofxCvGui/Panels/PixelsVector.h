#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "ofxLiquidEvent.h"

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
			int getSelectionIndex() const;
			void setSelectionIndex(int selection);
			ofxLiquidEvent<int> onSelectionChange;
		protected:
			void drawImage(float width, float height);
            void drawInfo();
            float getImageWidth() const;
            float getImageHeight() const;
			void key(KeyboardArguments &);
			void mouse(MouseArguments &);
			void arrangeButtons();
		private:
			const vector<ofPixels>& pixels;
			vector<ofRectangle> buttons;

			ofTexture preview;
			int selection;
			bool needsUpdate;
			size_t lastCount;
            bool refreshPerFrame;
		};

		shared_ptr<Panels::PixelsVector> makePixelsVector(const vector<ofPixels>& asset, string caption = "");
	}
}