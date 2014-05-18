#include "Slider.h"
#include "../../../addons/ofxAssets/src/ofxAssets.h"
#include "ofSystemUtils.h"

using namespace ofxAssets;

namespace ofxCvGui {
	namespace Widgets {
		//----------
		ofMesh * Slider::tenTicks = nullptr;

		//----------
		Slider::Slider(ofParameter<float> & parameter) {
			this->init();
			this->value = & parameter;
			this->setCaption(this->value->getName());
			this->mouseHover = false;

			this->onValueChange += [this] (ofParameter<float> & value) {
				if (this->validator) {
					auto validatorValue = value.get();
					this->validator(validatorValue);
					value.set(validatorValue);
				}
			};
		}

		//----------
		Slider::~Slider() {
		}

		//----------
		void Slider::setIntValidator() {
			this->setValidator([] (float & value) {
				value = floor(value + 0.5f);
			});
		}

		//----------
		void Slider::setValidator(Validator validator) {
			this->validator = validator;
		}

		//----------
		void Slider::init() {
			this->zoom = 1.0f;
			this->startMouseHoldTime = 0;
			this->startMouseHoldValue = 0;

			if (this->tenTicks == nullptr) {
				this->tenTicks = new ofMesh();
				auto & ticks = * this->tenTicks;
				for(float x = 0.0f; x< 1.0f; x+= 0.1f) {
					ticks.addVertex(ofVec3f(x, 0.0f, 0.0f));
				}
				ticks.setMode(OF_PRIMITIVE_POINTS);
			}

			this->setBounds(ofRectangle(5, 0, 100, 50));
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
			if (this->value->get() < this->value->getMin() || this->value->get() > this->value->getMax()) {
				this->value->set(ofClamp(this->value->get(), this->value->getMin(), this->value->getMax()));
				this->notifyValueChange();
			}

			if(this->getMouseState() == LocalMouseState::Down && this->mouseHeldOnBar) {
				const auto mouseHoldTime = float(ofGetElapsedTimeMillis() - startMouseHoldTime) / 1000.0f;
				if (mouseHoldTime > 1.0f) {
					this->zoom = exp(mouseHoldTime - 1.0f);
				}
			} else if (this->getMouseState() == LocalMouseState::Waiting) {
				this->zoom = (this->zoom - 1.0f) * 0.9f + 1.0f;
			}
		}

		//----------
		void Slider::draw(DrawArguments & args) {
			auto & font = ofxAssets::AssetRegister.getFont(ofxCvGui::defaultTypeface, 12);
			image("ofxCvGui::edit").draw(this->editBounds);
			font.drawString(this->value->getName() + " : " + ofToString(this->value->get()), 0, 15);
	
			const auto rangeScale = this->getRangeScale();
			const auto width = this->getWidth();
			const auto center = this->startMouseHoldValue;
			const auto centerPx = ofMap(center, this->value->getMin(), this->value->getMax(), 0, this->getWidth());
			const auto xPx = floor((this->value->get() - center) * (zoom * width) / rangeScale + centerPx);
	
			bool barActive = this->mouseHeldOnBar && this->getMouseState() != LocalMouseState::Waiting;

			if (this->mouseHover) {
				ofPushStyle();
				ofSetColor(50);
				ofRect(0, 20, xPx, 20);
				ofPopStyle();
			}

			ofPushStyle();
			ofSetColor(50);
			ofLine(0, 40, this->getWidth(), 40);
			ofPopStyle();

			ofPushMatrix();
			ofTranslate(0.0f, 40.0f);

			ofPushMatrix();
			ofTranslate(xPx, 0.0f);

			//draw handle
			auto & marker = image(barActive ? "ofxCvGui::sliderMarkerFilled" : "ofxCvGui::sliderMarker");
			ofScale(0.4f, 0.4f);
			marker.draw(-marker.getWidth() / 2.0f, -marker.getHeight() - 3);
			ofPopMatrix();
	
			//zoom around start drag value
			ofTranslate(+centerPx, 0.0f);
			ofScale(zoom, 1.0f);
			ofTranslate(-centerPx, 0.0f);
			ofScale(width, 1.0f);
																				// if range=255.0f
			const auto magnitudeOrder = ceil(log(rangeScale) / log(10));		// 3
			const auto magnitude = pow(10.0f, magnitudeOrder);					// 1000
			const auto innerMagnitude = pow(10.0f, magnitudeOrder - 2);			// 10
			const auto mantissa = rangeScale / magnitude;						// 0.255
			const auto majorTickCount = 10.0f * mantissa;						// 2.55
			const auto majorTickFraction = 1.0f / majorTickCount;				// 0.3921

			ofMesh majorTicks;
			//postitive ticks
			for(float tick = 0.0f; tick <= value->getMax(); tick += innerMagnitude) {
				majorTicks.addVertex(ofVec3f(tick, 0.0f, 0.0f));
				majorTicks.addColor(255);
			}
			//negative ticks
			for(float tick = 0.0f; tick >= value->getMin(); tick -= innerMagnitude) {
				if (tick == 0.0f) {
					continue;
				}
				majorTicks.addVertex(ofVec3f(tick, 0.0f, 0.0f));
				majorTicks.addColor(0);
			}

			ofScale(1.0f / rangeScale, 1.0f, 1.0f);
			ofTranslate(-value->getMin(), 0.0f, 0.0f);
			majorTicks.drawVertices();
	
			ofPopMatrix();

			//draw side marker
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
						if (ofGetElapsedTimeMillis() - this->startMouseHoldTime < 500 && abs(args.local.x - this->startMouseHoldMouseX) < 5) {
							//double click	
							this->value->set(ofMap(args.localNormalised.x, 0, 1.0f, this->value->getMin(), this->value->getMax(), true));
							this->notifyValueChange();
						}
						//start hold
						this->startMouseHoldTime = ofGetElapsedTimeMillis();
						this->startMouseHoldValue = this->value->get();
						this->startMouseHoldMouseX = args.local.x;
					} else if (this->editBounds.inside(args.local)) {
						//if we clicked the pencil
						auto result = ofSystemTextBoxDialog(this->value->getName() + " (" + ofToString(this->value->get()) + ")");
						if (!result.empty()) {
							this->value->set(ofToFloat(result));
							this->notifyValueChange();
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
					this->notifyValueChange();
					break;
				}
				break;
			case MouseArguments::Moved:
				this->mouseHover = args.isLocal() && args.local.y > 15;
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

		//----------
		void Slider::notifyValueChange() {
			this->onValueChange.notifyListeners(* this->value);
		}
	}
}