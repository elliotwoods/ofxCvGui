#include "ofxCvGui/Panels/Pixels.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Pixels::Pixels(ofPixels& pixels) :
		pixels(pixels) {
		}

		//----------
		void Pixels::update() {
			preview.loadData(pixels);
		}

		//----------
		void Pixels::drawImage(const DrawArguments& arguments) {
			this->preview.draw(this->getBounds());
		}
	}
}