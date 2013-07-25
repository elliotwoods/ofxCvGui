#include "ofxCvGui/Panels/Image.h"
namespace ofxCvGui {
	namespace Panels {
		//----------
		Image::Image(ofImage& image) :
		image(image) {
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
			this->image.draw(0, 0, width, height);
		}

        //----------
        void Image::drawInfo(ofxCvGui::DrawArguments & arguments) {
			if (!arguments.chromeEnabled)
				return;
            const ofPixels & pixels (image.getPixelsRef());
            
            stringstream ss;
            ss << pixels.getWidth() << "x" << pixels.getHeight() << ", " << pixels.getBitsPerChannel() << "bit/" << pixels.getNumChannels() << "ch";
            
            AssetRegister.drawText(ss.str(), 20, this->getHeight() - 30, "", true, 20);
        }
        
        //----------
        float Image::getImageWidth() const {
            return this->image.getWidth();
        }
        
        //----------
        float Image::getImageHeight() const {
            return this->image.getHeight();
        }
	}
}