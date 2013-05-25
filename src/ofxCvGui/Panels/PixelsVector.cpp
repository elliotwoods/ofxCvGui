#include "ofxCvGui/Panels/PixelsVector.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		PixelsVector::PixelsVector(const vector<ofPixels>& pixels) :
			pixels(pixels) {
			this->selection = -1;
			this->lastSelection = -1;
			this->lastCount = 0;
			this->refreshPerFrame = false;
            ofAddListener(this->onUpdate, this, &PixelsVector::update);
            ofAddListener(this->onDraw, this, &PixelsVector::drawInfo);
		}

        //----------
        PixelsVector::~PixelsVector() {
            ofRemoveListener(this->onUpdate, this, &PixelsVector::update);
            ofRemoveListener(this->onDraw, this, &PixelsVector::drawInfo);
        }
        
		//----------
		void PixelsVector::update(UpdateArguments&) {
			if (lastCount != pixels.size()) {
				this->selection = pixels.size() - 1;
				lastCount = pixels.size();
			}

			if (selection > pixels.size())
				selection = pixels.size() - 1;

			if (selection != lastSelection || refreshPerFrame) {
				if (selection >= 0) {
					const ofPixels& pixels(this->getSelection());
					if (preview.getWidth() != pixels.getWidth() || preview.getHeight() != pixels.getHeight())
						preview.allocate(pixels);
					preview.loadData(pixels);
					lastSelection = selection;
				} else
					preview.clear();
			}
		}

		//----------
		const ofPixels & PixelsVector::getSelection() const {
			return this->pixels[selection];
		}

		//----------
		void PixelsVector::drawImage(float width, float height) {
			preview.draw(0, 0, width, height);
        }
        
		//----------
		void PixelsVector::drawInfo(DrawArguments& arguments) {
			float x = 20;
			float y = this->getHeight() - 50;
			ofRectangle bounds;
			for (int i=0; i<this->pixels.size(); i++) {
				bounds = AssetRegister.drawText(ofToString(i), x, y, "", true, 30, 30);
				if (i==selection) {
					ofPushStyle();
					ofSetColor(255);
					ofSetLineWidth(1);
					ofNoFill();
					ofRect(bounds);
					ofPopStyle();
				}

				x += 40;
				if (x > this->getWidth() - 30) {
					x = 20;
					y -= 40;
				}
			}
		}

		//----------
		float PixelsVector::getImageWidth() const {
			return this->getSelection().getWidth();
		}

		//----------
		float PixelsVector::getImageHeight() const {
			return this->getSelection().getHeight();
		}
	}
}