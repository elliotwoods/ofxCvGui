#include "ofxCvGui/Panels/Draws.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Draws::Draws() {
			this->drawObject = nullptr;
		}

		//----------
		Draws::Draws(ofBaseDraws & drawObject) {
			this->setDrawObject(drawObject);
		}

		//----------
		void Draws::setDrawObject(ofBaseDraws & drawObject) {
			this->drawObject = & drawObject;
		}

		//----------
		void Draws::clearDrawObject() {
			this->drawObject = nullptr;
		}

		//----------
		ofBaseDraws * Draws::getDrawObject() const {
			return this->drawObject;
		}

		//----------
		void Draws::drawImage(float width, float height) {
			if (this->drawObject) {
				this->drawObject->draw(0, 0, width, height);
			}
		}
        
		//----------
        float Draws::getImageWidth() const {
			if (this->drawObject) {
				return this->drawObject->getWidth();
			}
			else {
				return 1;
			}
        }
        
		//----------
        float Draws::getImageHeight() const {
			if (this->drawObject) {
				return this->drawObject->getHeight();
			}
			else {
				return 1;
			}
        }
	}
}