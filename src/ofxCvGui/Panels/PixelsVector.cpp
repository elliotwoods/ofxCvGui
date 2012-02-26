#include "ofxCvGui/Panels/PixelsVector.h"

namespace ofxCvGui {
	extern Assets AssetRegister;
	namespace Panels {
		//----------
		PixelsVector::PixelsVector(const vector<ofPixels>& pixels) :
			pixels(pixels) {
			this->selection = 0;
			this->lastCount = 0;
		}

		//----------
		void PixelsVector::update() {
			if (lastCount != pixels.size()) {
				this->selection = pixels.size() - 1;
				lastCount = pixels.size();
			}

			if (pixels.size() > selection) {
				const ofPixels& pixels(this->pixels[selection]);
				if (preview.getWidth() != pixels.getWidth() || preview.getHeight() != pixels.getHeight())
					preview.allocate(pixels);
				preview.loadData(pixels);
			} else
				preview.clear();
		}

		//----------
		void PixelsVector::drawImage(const DrawArguments& arguments) {
			preview.draw(0, 0, this->getWidth(), this->getHeight());

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
	}
}