#include "Slider.h"
#include "../../../addons/ofxAssets/src/ofxAssets.h"

using namespace ofxAssets;

namespace ofxCvGui {
	namespace Widgets {
		//----------
		ofMesh * Slider::ticks = nullptr;

		//----------
		Slider::Slider(ofParameter<float> & parameter) {
			init();
			this->value = & parameter;
			this->valueLocallyAllocated = false;
		}

		//----------
		Slider::~Slider() {
			if (this->valueLocallyAllocated) {
				delete this->value;
			}
		}

		//----------
		void Slider::init() {
			this->zoom = 1.0f;
			this->startMouseHoldTime = 0;
			this->mouseHoldState = None;

			if (this->ticks == nullptr) {
				this->ticks = new ofMesh();
				auto & ticks = * this->ticks;
				for(float x = 0.0f; x<= 1.0f; x+= 0.01f) {
					ticks.addVertex(ofVec3f(x, 0.0f, 0.0f));
				}
				ticks.setMode(OF_PRIMITIVE_POINTS);
			}
		}

		//----------
		void Slider::update(UpdateArguments &) {
			this->value->set(ofClamp(this->value->get(), this->value->getMin(), this->value->getMax()));
			if(this->mouseHoldState == Hold) {
				const auto mouseHoldTime = float(ofGetElapsedTimeMillis() - startMouseHoldTime) / 1000.0f;
				if (mouseHoldTime > 1.0f) {
					zoom = mouseHoldTime;
				}
			} else if (this->mouseHoldState == None) {
				zoom = (zoom - 1.0f) * 0.9f + 1.0f;
			}
		}

		//----------
		void Slider::draw(DrawArguments & args) {
			auto & font = ofxAssets::AssetRegister.getFont("ofxCvGui::swisop3", 12);
			image("ofxCvGui::edit").draw(this->editBounds);
			font.drawString(this->caption + " : " + ofToString(this->value->get()), 5, 15);
	
			const auto rangeScale = this->value->getMax() - this->value->getMin();
			const auto width = this->getWidth();
			const auto center = this->startMouseHoldX;
			const auto centerPx = ofMap(center, this->value->getMin(), this->value->getMax(), 0, width);
			const auto xPx = (this->value->get() - center) * (zoom * width) / rangeScale + centerPx;
	
			ofPushMatrix();
			ofTranslate(0.0f, 40.0f);

			ofPushMatrix();
			ofTranslate(xPx, 0.0f);
			auto & marker = image("ofxCvGui::sliderMarker");
			marker.draw(-marker.getWidth() / 2.0f, -marker.getHeight());
			ofPopMatrix();
	

			ofTranslate(+centerPx, 0.0f);
			ofScale(zoom, 1.0f);
			ofTranslate(-centerPx, 0.0f);
			ofScale(width, 1.0f);
			this->ticks->draw();
	
			ofPopMatrix();
		}

		//----------
		void Slider::mouseAction(MouseArguments & args) {
		}

		//----------
		void Slider::boundsChange(BoundsChangeArguments & args) {
			this->editBounds = ofRectangle(args.bounds.width - 16,1,15, 15);
		}
	}
}