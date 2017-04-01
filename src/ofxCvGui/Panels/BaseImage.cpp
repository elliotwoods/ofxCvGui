#include "ofxCvGui/Panels/BaseImage.h"
#include "ofxAssets.h"

using namespace ofxAssets;

namespace ofxCvGui {	
	namespace Panels {
        //----------
		ImageZoomState BaseImage::getImageZoomState() const {
			return this->zoomState;
		}

		//----------
		void BaseImage::setMirror(bool mirror) {
			this->mirror = mirror;
		}


		//----------
		bool BaseImage::getMirror() const {
			return this->mirror;
		}


		//----------
		ofMatrix4x4 BaseImage::getPanelToImageTransform() const {
			float factor = this->getZoomFactor();

			switch (this->zoomState) {
			case ZoomX10:
			case ZoomX1:
			{
				ofMatrix4x4 transform;
				transform.scale(factor, factor, 1);
				transform.translate(-this->scroll); //scroll is in panel coordinates
				return transform;
			}

			case Stretch:
			{
				return ofMatrix4x4::newScaleMatrix(this->getWidth() / this->getImageWidth(), this->getHeight() / this->getImageHeight(), 1.0f);
			}
			case Fit:
			{
				auto aspectImage = this->getImageWidth() / this->getImageHeight();
				auto aspectPanel = this->getWidth() / this->getHeight();
				auto aspectSelection = aspectImage > aspectPanel;

				float scale = aspectSelection
					? this->getWidth() / this->getImageWidth() //letterbox top/bottom
					: this->getHeight() / this->getImageHeight(); //letterbox left/right

				ofMatrix4x4 transform;
				transform.scale(scale, scale, 1.0f);
				
				if (aspectSelection) {
					auto heightOfDrawnImage = this->getImageHeight() * scale;
					auto offsetFromTop = (this->getHeight() - heightOfDrawnImage) / 2.0f;
					transform.translate(0.0f, offsetFromTop, 0.0f);
				}
				else {
					auto widthOfDrawnImage = this->getImageWidth() * scale;
					auto offsetFromLeft = (this->getWidth() - widthOfDrawnImage) / 2.0f;
					transform.translate(offsetFromLeft, 0.0f, 0.0f);
				}
				return transform;
			}
			default:
				ofLogError() << "Zoom state not supported";
				return ofMatrix4x4();
			}
		}


		//----------
		float BaseImage::getZoomFactor() const {
			switch (this->zoomState) {
			case ZoomX10:
				return 10.0f;
			case ZoomX1:
				return 1.0f;
			default:
				return this->getWidth() / this->getImageWidth(); // shouldn't be here
			}
		}

		//----------
        BaseImage::BaseImage() {
			this->onDraw.addListener([this](DrawArguments & outerArgs) {
				//transform in and draw the image
				ofPushMatrix();
				{
					auto transform = this->getPanelToImageTransform();
					ofMultMatrix(transform);
					DrawImageArguments args{
						outerArgs
						, this->zoomState
						, ofRectangle(0, 0, this->getImageWidth(), this->getImageHeight())
					};
					this->onDrawImage.notifyListeners(args);
				}
				ofPopMatrix();
			}, this, -1);

			this->onDrawImage += [this](DrawImageArguments & args) {
				//draw the actual image from inherited class
				this->drawImage(args.drawBounds.width, args.drawBounds.height);
			};
		
			//toolbar
			{
				auto zoomFitStretch = this->addToolBarElement("ofxCvGui::zoom_fit_stretch", [this]() {
					this->zoomState = ImageZoomState::Stretch;
				});
				auto zoomFitWidth = this->addToolBarElement("ofxCvGui::zoom_fit_width", [this]() {
					this->zoomState = ImageZoomState::Fit;
				});
				auto zoomOne = this->addToolBarElement("ofxCvGui::zoom_one", [this]() {
					this->zoomState = ImageZoomState::ZoomX1;
				});
				auto zoomTen = this->addToolBarElement("ofxCvGui::zoom_ten", [this]() {
					this->zoomState = ImageZoomState::ZoomX10;
				});

				auto mirrorToggle = this->addToolBarElement("ofxCvGui::mirror", [this]() {
					this->mirror ^= true;
				});
				
				this->toolBar->onDraw += [=](DrawArguments & args){
					ofPushStyle();
					{
						ofSetColor(150);
						ofSetLineWidth(2.0f);
						ofNoFill();
						switch (this->zoomState) {
						case ImageZoomState::Stretch:
							ofDrawRectangle(zoomFitStretch->getBounds());
							break;
						case ImageZoomState::Fit:
							ofDrawRectangle(zoomFitWidth->getBounds());
							break;
						case ImageZoomState::ZoomX1:
							ofDrawRectangle(zoomOne->getBounds());
							break;
						case ImageZoomState::ZoomX10:
							ofDrawRectangle(zoomTen->getBounds());
							break;
						default:
							break;
						}

						if (this->mirror) {
							ofSetColor(255);
							ofxAssets::image("ofxCvGui::mirror_selected").draw(mirrorToggle->getBounds());
							ofSetColor(150);
							ofDrawRectangle(mirrorToggle->getBounds());
						}
					}
					ofPopStyle();
				};
			}
			
			
			//zoom box
			auto zoomBox = makeElement();
			{
				zoomBox->setBounds(ofRectangle(100, 100, 150, 100));
				
				auto zoomBoxWeak = weak_ptr<Element>(zoomBox);
				
				zoomBox->onDraw += [this](DrawArguments & args) {
					ofRectangle zoomBoxBounds = args.localBounds;
					ofRectangle zoomSelection;
					
					auto factor = this->getZoomFactor();
					zoomSelection.width = this->getWidth() / this->getImageWidth() / factor * zoomBoxBounds.width;
					zoomSelection.height = this->getHeight() / this->getImageHeight() / factor * zoomBoxBounds.height;
					ofVec2f zoomSelectionPos = -this->scroll / ofVec2f(this->getImageWidth(), this->getImageHeight()) / factor * ofVec2f(zoomBoxBounds.width, zoomBoxBounds.height);
					zoomSelection.x = zoomSelectionPos.x;
					zoomSelection.y = zoomSelectionPos.y;
					
					////
					//draw zoom box
					//
					
					//draw image
					{
						ofPushMatrix();
						{
							DrawImageArguments drawImageArgs{ args
								, this->zoomState
								, ofRectangle(0, 0, this->getImageWidth(), this->getImageHeight()) };

							ofScale(args.localBounds.width / this->getWidth(), args.localBounds.height / this->getHeight());

							ofMultMatrix(this->getPanelToImageTransform());
							this->onDrawImage.notifyListeners(drawImageArgs);
						}
						ofPopMatrix();
					}
					
					ofPushStyle();
					
					//draw outer
					ofSetColor(150);
					ofNoFill();
					ofSetLineWidth(2.0f);
					ofDrawRectangle(args.localBounds);
					
					//draw inner
					ofEnableAlphaBlending();
					ofFill();
					ofSetLineWidth(0.0f);
					ofSetColor(255, 255, 255, 100);
					ofDrawRectangle(zoomSelection);
					
					ofPopStyle();
					
					//
					////
				};
				
				zoomBox->onMouse += [this, zoomBoxWeak](MouseArguments & mouse) {
					auto zoomBox = zoomBoxWeak.lock();
					if(zoomBox) {
						mouse.takeMousePress(zoomBoxWeak.lock());
						if (mouse.isDragging(zoomBox)) {
							float factor = this->getZoomFactor();

							this->scroll -= mouse.movement / ofVec2f(zoomBox->getWidth(), zoomBox->getHeight()) * ofVec2f(this->getImageWidth(), this->getImageHeight()) * factor;
							this->clampScroll();
						}
					}
				};

				this->addChild(zoomBox);
			}
			
			this->onMouse.addListener([this](MouseArguments & args) {
				args.takeMousePress(this);
				if(args.isDragging(this)) {
					this->scroll -= args.movement;
					this->clampScroll();
				}
			}, this, -1);
			
			this->onUpdate += [this, zoomBox](UpdateArguments &) {
				//update zoom box
				{
					bool zoomBoxEnabled;
					float factor = this->getZoomFactor();
					switch (this->zoomState) {
					case ZoomX10:
					case ZoomX1:
						zoomBoxEnabled = this->getImageWidth() * factor > this->getWidth() || this->getImageHeight() * factor > this->getHeight();
						break;
					case Stretch:
					case Fit:
					default:
						zoomBoxEnabled = false;
						break;
					}

					zoomBox->setEnabled(zoomBoxEnabled);
					if(zoomBoxEnabled) {
						//set aspect ratio
						auto bounds = zoomBox->getBounds();
						bounds.height = this->getImageHeight() / this->getImageWidth() * bounds.width;
						bounds.x = this->getWidth() - bounds.width - 20;
						bounds.y = this->getHeight() - bounds.height - 20;
						zoomBox->setBounds(bounds);
					}
				}
			};
		}
        
        //----------
        BaseImage::~BaseImage() {
			this->onMouse.removeListeners(this);
		}
		
        //----------
        void BaseImage::nudgeZoom(KeyboardArguments& key) {
            if (key.checkCurrentPanel(this)) {
                float amount = 5.0f;
                switch (key.key) {
                    case OF_KEY_UP:
                        scroll.y += amount;
                        break;
                    case OF_KEY_DOWN:
                        scroll.y -= amount;
                        break;
                    case OF_KEY_LEFT:
                        scroll.x += amount;
                        break;
                    case OF_KEY_RIGHT:
                        scroll.x -= amount;
                        break;
                }
				this->clampScroll();
            }
        }

		//----------
		void BaseImage::clampScroll() {
			float factor = this->getZoomFactor();

			switch (this->zoomState) {
			case ZoomX1:
			case ZoomX10:
				break;

			case Stretch:
			case Fit:
			default:
				this->scroll = ofVec2f();
				return;
				break;
			}

			if (scroll.x < 0) {
				scroll.x = 0;
			}
			else {
				auto rightEdge = (scroll.x + this->getWidth()) / factor;
				if (rightEdge > this->getImageWidth() && this->getImageWidth() * factor > this->getWidth()) {
					scroll.x = this->getImageWidth() * factor - this->getWidth();
				}
			}

			if (scroll.y < 0) {
				scroll.y = 0;
			}
			else {
				auto bottomEdge = (scroll.y + this->getHeight()) / factor;
				if (bottomEdge > this->getImageHeight() && this->getImageHeight() * factor > this->getHeight()) {
					scroll.y = this->getImageHeight() * factor - this->getHeight();
				}
			}
		}
	}
}