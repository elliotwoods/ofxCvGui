#include "ofxCvGui/Panels/BaseDraws.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		BaseDraws::BaseDraws(ofBaseDraws& draws) :
		draws(draws) {
		}

		//----------
		void BaseDraws::customDraw(const DrawArguments& arguments) {
			this->draws.draw(this->getBounds());
		}
	}
}