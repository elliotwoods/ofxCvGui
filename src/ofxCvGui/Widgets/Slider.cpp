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
			this->mouseWentDownOnSlider = false;
		}

		//----------
		Slider::Slider(ofParameter<float> & parameter, Slider::ValueChangeCallback onValueChange) :
		Slider(parameter) {
			this->onValueChange += onValueChange;
		}

		//----------
		Slider::~Slider() {

		}

		//----------
		void Slider::addIntValidator() {
			this->addValidator([] (float & value) {
				value = floor(value + 0.5f);
			});
		}

		//----------
		void Slider::addStepValidator(float step) {
			this->addValidator([step] (float & value) {
				value = step * floor(value / step + 0.5f);
			});
		}
		
		//----------
		void Slider::addValidator(Validator validator) {
			this->validators.push_back(validator);
		}

		//----------
		void Slider::clearValidators() {
			this->validators.clear();
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

			this->setBounds(ofRectangle(5, 0, 100, 55));
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

			this->markViewDirty();
		}

		//----------
		void Slider::update(UpdateArguments &) {

			//Perform the zoom;
			if(this->getMouseState() == LocalMouseState::Down && this->mouseWentDownOnSlider) {
				const auto mouseHoldTime = float(ofGetElapsedTimeMillis() - startMouseHoldTime) / 1000.0f;
				if (mouseHoldTime > 1.0f) {
					this->zoom = exp(mouseHoldTime - 1.0f);
				}
			} else if (this->getMouseState() == LocalMouseState::Waiting) {
				this->zoom = (this->zoom - 1.0f) * 0.9f + 1.0f;
			}
			if(abs(this->zoom - 1.0f) > 1E-2) {
				this->markViewDirty();
			} else {
				this->zoom = 1.0f;
			}
		}

		//----------
		void Slider::draw(DrawArguments & args) {
			auto & font = ofxAssets::font(ofxCvGui::defaultTypeface, 13);
			image("ofxCvGui::edit").draw(this->editBounds);

			
			//--
			//Draw value string
			//--
			//
			stringstream valueString;
			valueString.imbue(std::locale(""));
			const auto actualValue = this->value->get();
			if (actualValue == floor(actualValue)) {
				valueString << std::fixed;
				valueString.precision(0);
			}
			else {
				valueString.precision(3);
			}
			valueString << actualValue;
			font.drawString(this->value->getName() + " : " + valueString.str(), 0, 15);
			//
			//--
			
			
			const auto rangeScale = this->getRangeScale();
			const auto width = this->getWidth();
			const auto center = this->startMouseHoldValue;
			
			if (rangeScale > 0) {
				const auto centerPx = ofMap(center, this->value->getMin(), this->value->getMax(), 0, this->getWidth());
				
				const auto valueToPosition = [&](const float & value) {
					return (value - center) * (zoom * width) / rangeScale + centerPx;
				};
				const auto positionToValue = [&](const float & position) {
					return (position - centerPx) * rangeScale / (zoom * width) + center;
				};

				const auto xPx = floor(valueToPosition(this->value->get()));
				const auto zeroPx = valueToPosition(0.0f);
				
				bool draggingSlider = this->mouseWentDownOnSlider && this->getMouseState() != LocalMouseState::Waiting;

				if (this->mouseHover) {
					ofPushStyle();
					ofSetColor(zeroPx < xPx ? 50 : 30);
					ofDrawRectangle(zeroPx, 20, xPx - zeroPx, 20);;
					
					//we draw as shapes because sometimes we miss the finer features (e.g. "-") with standard funcs
					
					ofSetColor(150);
					
					auto minValue = this->getCheckedValue(positionToValue(0.0f));
					auto & smallFont = ofxAssets::font(ofxCvGui::defaultTypeface, 10);
					smallFont.drawStringAsShapes(ofToString(minValue), 0, 51);

					auto maxValue = this->getCheckedValue(positionToValue(width));
					auto maxTextString = ofToString(maxValue);
					auto maxTextBounds = smallFont.getStringBoundingBox(maxTextString, 0.0f, 0.0f);
					smallFont.drawStringAsShapes(maxTextString, this->getWidth() - maxTextBounds.getRight(), 51);
					
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
				auto & marker = image(draggingSlider ? "ofxCvGui::sliderMarkerFilled" : "ofxCvGui::sliderMarker");
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
				if (innerMagnitude > 0.0f) {
					//postitive ticks
					for (float tick = 0.0f; tick <= value->getMax(); tick += innerMagnitude) {
						majorTicks.addVertex(ofVec3f(tick, 0.0f, 0.0f));
						majorTicks.addColor(255);
					}
					//negative ticks
					for (float tick = 0.0f; tick >= value->getMin(); tick -= innerMagnitude) {
						if (tick == 0.0f) {
							continue;
						}
						majorTicks.addVertex(ofVec3f(tick, 0.0f, 0.0f));
						majorTicks.addColor(0);
					}
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
		}

		//----------
		void Slider::mouseAction(MouseArguments & args) {
			switch (args.action) {
			case MouseArguments::Pressed:
				if (args.local.y > 15) {
					if (args.takeMousePress(this)) {
						this->mouseWentDownOnSlider = true;

						//start hold
						this->startMouseHoldTime = ofGetElapsedTimeMillis();
						this->startMouseHoldValue = this->value->get();
						this->startMouseHoldMouseX = args.local.x;
					}
				} else if (this->editBounds.inside(args.local)) {
					if (args.takeMousePress(this)) {
						this->mouseWentDownOnSlider = false;
						auto result = ofSystemTextBoxDialog(this->value->getName() + " (" + ofToString(this->value->get()) + ")");
						if (!result.empty()) {
							this->value->set(ofToFloat(result));
							this->checkValueAndNotifyListeners();
						}
					}
				}
				break;
			case MouseArguments::DoubleClick:
				if (args.getOwner() == this) {
					this->value->set(ofMap(args.localNormalised.x, 0, 1.0f, this->value->getMin(), this->value->getMax(), true));
					this->checkValueAndNotifyListeners();
				}
				break;
			case MouseArguments::Dragged:
				if (this->mouseWentDownOnSlider) {
					float dNormX = (args.local.x - this->startMouseHoldMouseX) / (this->getWidth() * zoom);
					this->value->set(dNormX * this->getRangeScale() + startMouseHoldValue);
					this->checkValueAndNotifyListeners();
				}
				break;
			case MouseArguments::Moved:
				{
					auto newMouseHover = args.isLocal() && args.local.y > 15;
					if(newMouseHover != this->mouseHover) {
						this->mouseHover = newMouseHover;
						this->markViewDirty();
					}
				}
				break;
			default:
				break;
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
		float Slider::getCheckedValue(float value) {
			for(auto & validator : this->validators) {
				validator(value);
			}
			
			return ofClamp(value, this->value->getMin(), this->value->getMax());
		}
		
		//----------
		void Slider::checkValueAndNotifyListeners() {
			this->value->set(this->getCheckedValue(this->value->get()));
			this->onValueChange.notifyListeners(* this->value);
			this->markViewDirty();
		}
	}
}