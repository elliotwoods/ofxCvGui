#include "ofxCvGui/Panels/Image.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Image::Image(ofAbstractImage & asset) {
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
		void Image::setImage(ofAbstractImage & asset) {
			this->asset = & asset;
		}

		//----------
		ofAbstractImage * Image::getImage() {
			return this->asset;
		}

        //----------
		void Image::drawImage(float width, float height) {
			if (this->asset) {
				if (this->asset->getWidth() != 0) {
					this->asset->draw(0, 0, width, height);
				}
			}
		}

        //----------
        void Image::drawInfo(ofxCvGui::DrawArguments & arguments) {
			if (this->asset) {
				if (!arguments.chromeEnabled)
					return;

				int bitsPerChannel = 0;
				int channelCount = 0;
				if (dynamic_cast<ofImage *>(this->asset)) {
					bitsPerChannel = 8;
					channelCount = dynamic_cast<ofImage *>(this->asset)->getPixels().getNumChannels();
				}
				else if (dynamic_cast<ofFloatImage *>(this->asset)) {
					bitsPerChannel = 32;
					channelCount = dynamic_cast<ofFloatImage *>(this->asset)->getPixels().getNumChannels();
				}
				else if (dynamic_cast<ofShortImage *>(this->asset)) {
					bitsPerChannel = 16;
					channelCount = dynamic_cast<ofShortImage *>(this->asset)->getPixels().getNumChannels();
				}

				stringstream ss;
				ss << this->asset->getWidth() << "x" << this->asset->getHeight();
				if (bitsPerChannel != 0) {
					ss << ", " << bitsPerChannel << "bit/" << channelCount << "ch";
				}
            
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