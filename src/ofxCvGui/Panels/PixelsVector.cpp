#include "ofxCvGui/Panels/PixelsVector.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		PixelsVector::PixelsVector(const vector<ofPixels>& pixels) :
			pixels(pixels) {
			this->selection = -1;
			this->needsUpdate = true;
			this->lastCount = 0;
			this->refreshPerFrame = false;
			this->onUpdate.addListener([this] (UpdateArguments&) { this->update(); }, this);
			this->onDraw.addListener([this] (DrawArguments&) { this->drawInfo(); }, this);
			this->onKeyboard.addListener([this] (KeyboardArguments& args) { this->key(args); }, this);
			this->onMouse.addListener([this] (MouseArguments& args) { this->mouse(args); }, this);
			this->onBoundsChange.addListener([this] (BoundsChangeArguments& args) { this->arrangeButtons(); }, this);
			this->onSelectionChange.addListener([this] (int index) { this->needsUpdate = true; }, this);
		}

        //----------
        PixelsVector::~PixelsVector() {
			this->onUpdate.removeListeners(this);
			this->onDraw.removeListeners(this);
			this->onKeyboard.removeListeners(this);
			this->onMouse.removeListeners(this);
			this->onBoundsChange.removeListeners(this);
			this->onSelectionChange.removeListeners(this);
        }
        
		//----------
		void PixelsVector::update() {
			if (lastCount < pixels.size()) {
				this->selection = (int) pixels.size() - 1;
				this->onSelectionChange(selection);
				lastCount = pixels.size();
			} else if (lastCount > pixels.size()) {
				this->selection = 0;
				this->onSelectionChange(selection);
			}

			if ((int) pixels.size() <= selection) {
				selection = pixels.size() == 0 ? -1 : 0;
				this->onSelectionChange(selection);
			}

			if (this->needsUpdate) {
				if (selection >= 0) {
					const ofPixels& pixels(this->getSelection());
					if (preview.getWidth() != pixels.getWidth() || preview.getHeight() != pixels.getHeight())
						preview.allocate(pixels);
					preview.loadData(pixels);
				} else
					preview.clear();
				this->needsUpdate = false;
			}
		}

		//----------
		const ofPixels & PixelsVector::getSelection() const {
			return this->pixels[selection];
		}

		//----------
		int PixelsVector::getSelectionIndex() const {
			return this->selection;
		}

		//----------
		void PixelsVector::setSelectionIndex(int selection) {
			this->selection = selection;
			this->onSelectionChange(selection);
		}

		//----------
		void PixelsVector::drawImage(float width, float height) {
			preview.draw(0, 0, width, height);
        }
        
		//----------
		void PixelsVector::drawInfo() {
			ofRectangle bounds;
			if (pixels.size() != buttons.size()) {
				arrangeButtons();
			}
			for (int i=0; i<this->pixels.size(); i++) {
				auto & button = buttons[i];
				bounds = Utils::drawText(ofToString(i), button.x, button.y, true, 30, 30);
				if (i==selection) {
					ofPushStyle();
					ofSetColor(255);
					ofSetLineWidth(1);
					ofNoFill();
					ofDrawRectangle(bounds);
					ofPopStyle();
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

		//----------
		void PixelsVector::key(KeyboardArguments & args) {
			if (args.checkCurrentPanel(this)) {
				auto count = static_cast<int>(this->pixels.size());
				switch (args.key)
				{
				case OF_KEY_LEFT:
					if (count > 0) {
						selection--;
						if (selection < 0)
							selection += count;
						this->onSelectionChange(selection);
					}
					break;
				case OF_KEY_RIGHT:
					if (count > 0) {
						selection++;
						selection %= count;
						this->onSelectionChange(selection);
					}
					break;
				default:
					break;
				}
			}
		}
		
		//----------
		void PixelsVector::mouse(MouseArguments & args) {
			for(int i=0; i<this->buttons.size(); i++) {
				if (this->buttons[i].inside(args.local)) {
					if (args.takeMousePress(this)) {
						this->selection = i;
						this->onSelectionChange(selection);
					}
				}
			}
		}

		//----------
		void PixelsVector::arrangeButtons() {
			this->buttons.clear();
			float x = 20;
			float y = this->getHeight() - 50;
			for(size_t i=0; i<pixels.size(); i++) {
				buttons.push_back(ofRectangle(x, y, 30, 30));
				x += 40;
				if (x > this->getWidth() - 30) {
					x = 20;
					y -= 40;
				}
			}
		}

		//----------
		shared_ptr<Panels::PixelsVector> makePixelsVector(const vector<ofPixels>& asset, string caption) {
			auto newPanel = make_shared<Panels::PixelsVector>(asset);
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}