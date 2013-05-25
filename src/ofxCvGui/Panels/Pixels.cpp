#include "ofxCvGui/Panels/Pixels.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Pixels::Pixels(const ofPixels& pixels) :
		pixels(pixels) {
			this->refreshPerFrame = true;
            ofAddListener(this->onUpdate, this, &Pixels::update);
		}
        
        //----------
        Pixels::~Pixels() {
            ofRemoveListener(this->onUpdate, this, &Pixels::update);
        }

		//----------
		void Pixels::update(UpdateArguments&) {
			if (refreshPerFrame) {
				if (preview.getWidth() != pixels.getWidth() || preview.getHeight() != pixels.getHeight())
					preview.allocate(pixels);
				preview.loadData(pixels);
			}
		}

		//----------
		void Pixels::drawImage(float width, float height) {
			this->preview.draw(0, 0, this->getWidth(), this->getHeight());
		}

		//----------
		float Pixels::getImageWidth() const {
			return this->pixels.getWidth();
		}

		//----------
		float Pixels::getImageHeight() const {
			return this->pixels.getHeight();
		}
	}
}