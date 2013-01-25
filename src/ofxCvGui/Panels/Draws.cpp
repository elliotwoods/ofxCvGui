#include "ofxCvGui/Panels/Draws.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Draws::Draws(ofBaseDraws& draws) :
		draws(draws) {
		}

		//----------
		void Draws::drawImage(float width, float height) {
            this->draws.draw(0, 0, width, height);
		}
        
		//----------
        float Draws::getImageWidth() const {
            return this->draws.getWidth();
        }
        
		//----------
        float Draws::getImageHeight() const {
            return this->draws.getHeight();
        }
	}
}