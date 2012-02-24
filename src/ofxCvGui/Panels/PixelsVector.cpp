#include "ofxCvGui/Panels/PixelsVector.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		PixelsVector::PixelsVector(const vector<ofPixels>& pixels) :
			pixels(pixels) {
		}

		//----------
		void PixelsVector::update() {
			if (pixels.size() > 0) {
				preview.loadData(pixels[0]);
			}
		}

		//----------
		void PixelsVector::drawImage(const DrawArguments& arguments) {
			preview.draw(0, 0, this->getWidth(), this->getHeight());
		}
	}
}