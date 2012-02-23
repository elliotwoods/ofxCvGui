#pragma once
#pragma once
#include "ofxCvGui/Element.h"
#include "ofPixels.h"
#include "ofTexture.h"

namespace ofxCvGui {
	namespace Panels {
		class Pixels : public Element {
		public:
			void update();
		protected:
			Pixels(ofPixels& pixels);
			void customDraw(const DrawArguments& arguments);
		private:
			ofPixels& pixels;
			ofTexture preview;
		};
	}
}