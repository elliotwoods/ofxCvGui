#include "pch_ofxCvGui.h"


namespace ofxCvGui {
	namespace Panels {
		//----------
		Image::Image(ofAbstractImage & asset) {
			this->asset = 0;
			this->setImage(asset);
			this->onDraw.addListener([this] (DrawArguments& args) {
				this->drawInfo(args);
			}, this);
			
			this->addToolBarElement("ofxCvGui::load", [this]() {
				if(!this->asset) {
					return;
				}
				auto result = ofSystemLoadDialog("Load image");
				if (result.bSuccess) {
					if (dynamic_cast<ofImage *>(this->asset)) {
						static_cast<ofImage *>(this->asset)->load(result.filePath);
					}
					if (dynamic_cast<ofShortImage *>(this->asset)) {
						static_cast<ofShortImage *>(this->asset)->load(result.filePath);
					}
					if (dynamic_cast<ofFloatImage *>(this->asset)) {
						static_cast<ofFloatImage *>(this->asset)->load(result.filePath);
					}
				}
			});
			
			this->addToolBarElement("ofxCvGui::save", [this]() {
				if(!this->asset) {
					return;
				}
				auto defaultFilename = this->caption.empty() ? "image.png" : this->caption + ".png";
				auto result = ofSystemSaveDialog(defaultFilename, "Save image");
				if (result.bSuccess) {
					if (dynamic_cast<ofImage *>(this->asset)) {
						static_cast<ofImage *>(this->asset)->save(result.filePath);
					}
					if (dynamic_cast<ofShortImage *>(this->asset)) {
						static_cast<ofShortImage *>(this->asset)->save(result.filePath);
					}
					if (dynamic_cast<ofFloatImage *>(this->asset)) {
						static_cast<ofFloatImage *>(this->asset)->save(result.filePath);
					}
				}
			});
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

				bool unknownType = false;
				bool allocated = false;
				int bitsPerChannel = 0;
				size_t channelCount = 0;

				if (dynamic_cast<ofImage *>(this->asset)) {
					const auto & pixels = dynamic_cast<ofImage *>(this->asset)->getPixels();
					allocated = pixels.isAllocated();
					if (allocated) {
						channelCount = pixels.getNumChannels();
						bitsPerChannel = 8;
					}
				}
				else if (dynamic_cast<ofFloatImage *>(this->asset)) {
					const auto & pixels = dynamic_cast<ofFloatImage *>(this->asset)->getPixels();
					allocated = pixels.isAllocated();
					if (allocated) {
						channelCount = pixels.getNumChannels();
						bitsPerChannel = 32;
					}
				}
				else if (dynamic_cast<ofShortImage *>(this->asset)) {
					const auto & pixels = dynamic_cast<ofShortImage *>(this->asset)->getPixels();
					allocated = pixels.isAllocated();
					if (allocated) {
						channelCount = pixels.getNumChannels();
						bitsPerChannel = 16;
					}
				}
				else {
					unknownType = true;
				}

				stringstream ss;
				ss << this->asset->getWidth() << "x" << this->asset->getHeight();
				if (unknownType) {
					ss << ", unknown type";
				}
				else if (!allocated) {
					ss << ", unallocated";
				}
				else {
					ss << ", " << bitsPerChannel << "bit, " << channelCount << "ch";
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

		//----------
		shared_ptr<Panels::Image> makeImage(ofImage& asset, string caption) {
			auto newPanel = make_shared<Panels::Image>(asset);
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}