#include "ofxCvGui/Panels/Image.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Image::Image(ofImage & asset) :
		asset(asset) {
			this->onDraw.addListener([this] (DrawArguments& args) {
				this->drawInfo(args);
			}, this);
		}
        
		//----------
        Image::~Image() {
			this->onDraw.removeListeners(this);
        }
		
        //----------
		void Image::drawImage(float width, float height) {
			if (this->asset.isAllocated()) {
				this->asset.draw(0, 0, width, height);
			}
		}

        //----------
        void Image::drawInfo(ofxCvGui::DrawArguments & arguments) {
			if (!arguments.chromeEnabled)
				return;
            const auto & pixels = this->asset.getPixelsRef();
            
            stringstream ss;
            ss << pixels.getWidth() << "x" << pixels.getHeight() << ", " << pixels.getBitsPerChannel() << "bit/" << pixels.getNumChannels() << "ch";
            
            Utils::drawText(ss.str(), 20, this->getHeight() - 30, true, 20);
        }
        
        //----------
        float Image::getImageWidth() const {
            return this->asset.getWidth();
        }
        
        //----------
        float Image::getImageHeight() const {
            return this->asset.getHeight();
        }
	}
}