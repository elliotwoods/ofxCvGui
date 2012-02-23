#pragma once
#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofPixels.h"
#include "ofTexture.h"

namespace ofxCvGui {
	namespace Panels {
		class Pixels : public Panels::Base {
		public:
			Pixels(ofPixels& pixels);
			void update();
		protected:
			void drawPanel(const DrawArguments& arguments);
		private:
			ofPixels& pixels;
			ofTexture preview;
		};
	}
}