#include "ofxCvGui/Panels/Pixels.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Pixels::Pixels(const ofPixels& pixels) :
		pixels(pixels) {
			this->refreshPerFrame = true;
			this->onUpdate.addListener([this] (UpdateArguments&) { this->updateImage(); }, this);
		}
        
        //----------
        Pixels::~Pixels() {
			this->onUpdate.removeListeners(this);
        }

		//----------
		void Pixels::updateImage() {
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