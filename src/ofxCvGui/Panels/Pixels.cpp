#include "ofxCvGui/Panels/Pixels.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Pixels::Pixels(const ofPixels& pixels) :
		pixels(pixels) {
		}

		//----------
		void Pixels::update() {
			if (preview.getWidth() != pixels.getWidth() || preview.getHeight() != pixels.getHeight())
				preview.allocate(pixels);
			preview.loadData(pixels);
		}

		//----------
		void Pixels::drawImage(const DrawArguments& arguments) {
			this->preview.draw(0, 0, this->getWidth(), this->getHeight());
		}
	}
}