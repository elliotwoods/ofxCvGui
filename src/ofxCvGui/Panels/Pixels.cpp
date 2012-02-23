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
		void Pixels::drawPanel(const DrawArguments& arguments) {
			this->preview.draw(this->getBounds());
		}
	}
}