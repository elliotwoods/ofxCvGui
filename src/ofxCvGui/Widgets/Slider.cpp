#include "Slider.h"
#include "../../../addons/ofxAssets/src/ofxAssets.h"
#include "ofSystemUtils.h"

using namespace ofxAssets;

namespace ofxCvGui {
	namespace Widgets {
		//----------
		ofMesh * Slider::ticks = nullptr;

		//----------
		Slider::Slider(ofParameter<float> & parameter) {
			this->init();
			this->value = & parameter;
		}

		//----------
		Slider::~Slider() {
		}

		//----------
		void Slider::init() {
			this->zoom = 1.0f;
			this->startMouseHoldTime = 0;
			this->startMouseHoldValue = 0;

			if (this->ticks == nullptr) {
				this->ticks = new ofMesh();
				auto & ticks = * this->ticks;
				for(float x = 0.0f; x<= 1.0f; x+= 0.02f) {
					ticks.addVertex(ofVec3f(x, 0.0f, 0.0f));
				}
				ticks.setMode(OF_PRIMITIVE_POINTS);
			}

			this->setBounds(ofRectangle(0, 0, 100, 50));
			this->onUpdate += [this] (UpdateArguments & args) {
				this->update(args);
			};
			this->onDraw += [this] (DrawArguments & args) {
				this->draw(args);
			};
			this->onMouse += [this] (MouseArguments & args) {
				this->mouseAction(args);
			};
			this->onBoundsChange += [this] (BoundsChangeArguments & args) {
				this->boundsChange(args);
			};

		}

		//----------
		void Slider::update(UpdateArguments &) {
			this->value->set(ofClamp(this->value->get(), this->value->getMin(), this->value->getMax()));
			if(this->getMouseState() == LocalMouseState::Down && this->mouseHeldOnBar) {
				const auto mouseHoldTime = float(ofGetElapsedTimeMillis() - startMouseHoldTime) / 1000.0f;
				if (mouseHoldTime > 1.0f) {
					this->zoom = mouseHoldTime;
				}
			} else if (this->getMouseState() == LocalMouseState::Waiting) {
				this->zoom = (this->zoom - 1.0f) * 0.9f + 1.0f;
			}
		}

		//----------
		void Slider::draw(DrawArguments & args) {
			auto & font = ofxAssets::AssetRegister.getFont("ofxCvGui::swisop3", 12);
			image("ofxCvGui::edit").draw(this->editBounds);
			font.drawString(this->value->getName() + " : " + ofToString(this->value->get()), 4, 15);
	
			const auto rangeScale = this->getRangeScale();
			const auto width = this->getWidth();
			const auto center = this->startMouseHoldValue;
			const auto centerPx = ofMap(center, this->value->getMin(), this->value->getMax(), 0, this->getWidth());
			const auto xPx = floor((this->value->get() - center) * (zoom * width) / rangeScale + centerPx);
	
			bool barActive = this->mouseHeldOnBar && this->getMouseState() != LocalMouseState::Waiting;

			if (barActive) {
				ofPushStyle();
				ofSetColor(50);
				ofRect(0, 20, xPx, 20);
				ofPopStyle();
			}

			ofPushMatrix();
			ofTranslate(0.0f, 40.0f);

			ofPushMatrix();
			ofTranslate(xPx, 0.0f);
			auto & marker = image(barActive ? "ofxCvGui::sliderMarkerFilled" : "ofxCvGui::sliderMarker");
			ofScale(0.4f, 0.4f);
			marker.draw(-marker.getWidth() / 2.0f, -marker.getHeight() - 3);
			ofPopMatrix();
	

			ofTranslate(+centerPx, 0.0f);
			ofScale(zoom, 1.0f);
			ofTranslate(-centerPx, 0.0f);
			ofScale(width, 1.0f);
			this->ticks->draw();
	
			ofPopMatrix();

			ofPushStyle();
			ofSetLineWidth(1.0f);
			ofLine(this->getWidth(), 0, this->getWidth(), 40);
			ofPopStyle();
		}

		//----------
		void Slider::mouseAction(MouseArguments & args) {
			switch (args.action) {
			case MouseArguments::Pressed:
				{
					bool nothingHappened = false;
					this->mouseHeldOnBar = args.local.y > 15;

					if (this->mouseHeldOnBar) {
						if (ofGetElapsedTimeMillis() - startMouseHoldTime < 500) {
							//double click	
							this->value->set(ofMap(args.localNormalised.x, 0, 1.0f, this->value->getMin(), this->value->getMax(), true));
						}

						//start hold
						this->startMouseHoldTime = ofGetElapsedTimeMillis();
						this->startMouseHoldValue = this->value->get();
						this->startMouseHoldMouseX = args.local.x;
					} else if (this->editBounds.inside(args.local)) {
						//if we clicked the pencil
						auto result = ofSystemTextBoxDialog(this->caption + " (" + ofToString(this->value->get()) + ")");
						if (!result.empty()) {
							this->value->set(ofToFloat(result));
						}
					} else {
						nothingHappened = true;
					}
					if (!nothingHappened) {
						args.take();
					}
					break;
				}
			case MouseArguments::Dragged:
				if (this->getMouseState() == LocalMouseState::Dragging && this->mouseHeldOnBar) {
					float dNormX = (args.local.x - this->startMouseHoldMouseX) / (this->getWidth() * zoom);
					this->value->set(dNormX * this->getRangeScale() + startMouseHoldValue);
					break;
				}
			}
		}

		//----------
		void Slider::boundsChange(BoundsChangeArguments & args) {
			this->editBounds = ofRectangle(args.bounds.width - 20,1,15, 15);
		}

		//----------
		float Slider::getRangeScale() const {
			return this->value->getMax() - this->value->getMin();
		}
	}
}