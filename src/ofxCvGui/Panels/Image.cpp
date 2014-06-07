#include "ofxCvGui/Panels/Image.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Image::Image(ofImage & asset) {
			this->asset = 0;
			this->setImage(asset);
			this->onDraw.addListener([this] (DrawArguments& args) {
				this->drawInfo(args);
			}, this);
		}
        
		//----------
        Image::~Image() {
			this->onDraw.removeListeners(this);
        }
		
        //----------
		void Image::setImage(ofImage & asset) {
			this->asset = & asset;
		}

        //----------
		void Image::drawImage(float width, float height) {
			if (this->asset) {
				if (this->asset->isAllocated()) {
					this->asset->draw(0, 0, width, height);
				}
			}
		}

        //----------
        void Image::drawInfo(ofxCvGui::DrawArguments & arguments) {
			if (this->asset) {
				if (!arguments.chromeEnabled)
					return;
				const auto & pixels = this->asset->getPixelsRef();
            
				stringstream ss;
				ss << pixels.getWidth() << "x" << pixels.getHeight() << ", " << pixels.getBitsPerChannel() << "bit/" << pixels.getNumChannels() << "ch";
            
				Utils::drawText(ss.str(), 20, this->getHeight() - 30, true, 20);
			}
        }
        
        //----------
        float Image::getImageWidth() const {
			if (this->asset) {
				return this->asset->getWidth();
			} else {
				return 0.0f;
			}
        }
        
        //----------
        float Image::getImageHeight() const {
            if (this->asset) {
				return this->asset->getHeight();
			} else {
				return 0.0f;
			}
        }
	}
}