#include "ofxCvGui/Panels/Pixels.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Pixels::Pixels(const ofPixels& pixels) :
		pixels(pixels) {
			this->refreshPerFrame = true;
			this->onUpdate.addListener([this] (UpdateArguments&) { this->updateImage(); }, this);
		
			this->onDraw.addListener([this] (DrawArguments& args) {
				this->drawInfo(args);
			}, this);
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
        void Pixels::drawInfo(ofxCvGui::DrawArguments & arguments) {
			if (!arguments.chromeEnabled)
				return;
            
            stringstream ss;
            ss << pixels.getWidth() << "x" << pixels.getHeight() << ", " << pixels.getBitsPerChannel() << "bit/" << pixels.getNumChannels() << "ch";
            
            Utils::drawText(ss.str(), 20, this->getHeight() - 30, true, 20);
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