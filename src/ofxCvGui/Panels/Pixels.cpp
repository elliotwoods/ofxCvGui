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
		void Pixels::customDraw(const DrawArguments& arguments) {
			this->preview.draw(this->getBounds());
		}
	}
}