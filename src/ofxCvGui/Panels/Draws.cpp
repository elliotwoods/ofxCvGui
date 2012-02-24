#include "ofxCvGui/Panels/Draws.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Draws::Draws(ofBaseDraws& draws) :
		draws(draws) {
		}

		//----------
		void Draws::drawImage(const DrawArguments& arguments) {
			this->draws.draw(0, 0, this->getWidth(), this->getHeight());
		}
	}
}