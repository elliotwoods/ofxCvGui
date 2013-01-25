#pragma once
#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "types/ofBaseTypes.h"

namespace ofxCvGui {
	namespace Panels {
		class Draws : public Panels::BaseImage {
		public:
			Draws(ofBaseDraws& pixels);
		protected:
			void drawImage(float width, float height);
            float getImageWidth() const;
            float getImageHeight() const;
		private:
			ofBaseDraws& draws;
		};
	}
}