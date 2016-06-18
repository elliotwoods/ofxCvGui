#include "ofxCvGui/Panels/BaseImage.h"
#include "ofxAssets.h"

using namespace ofxAssets;

namespace ofxCvGui {
	//----------
	DrawImageArguments::DrawImageArguments(bool zoomed, const ofVec2f & drawSize, const ofVec2f & viewSize, const ofVec2f & offsetCropped) {
		this->zoomed = zoomed;
		this->drawSize = drawSize;
		this->viewSize = viewSize;
		this->offsetCropped = offsetCropped;
	}
	
	namespace Panels {
        //----------
		BaseImage::Zoomed BaseImage::getZoomed() const {
			return this->zoom;
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
			ofMatrix4x4 transform;
			switch (this->zoom) {
				case ZoomOne:
				{
					transform.makeTranslationMatrix(-scroll);
					break;
				}
				case ZoomFit:
				{
					transform.makeScaleMatrix(this->getImageWidth() / this->getWidth(), this->getImageHeight() / this->getHeight(), 1.0f);
					break;
				}
				default:
				{
					ofLogWarning("ofxCvGui::BaseImage::getViewToImageTransform") << "Zoom level not supported";
				}
			}
			return transform;
		}

		//----------
        BaseImage::BaseImage() {
			this->onDraw.addListener([this] (DrawArguments & args) {
				this->drawImage(args);
			}, this, -1);

            this->zoom = ZoomFit;

			this->onDrawImage += [this](ofxCvGui::DrawImageArguments & args) {
				this->drawImage(args.drawSize.x, args.drawSize.y);
			};
		
			//toolbar
			{
				auto zoomFit = this->addToolBarElement("ofxCvGui::zoom_fit", [this]() {
					this->zoom = Zoomed::ZoomFit;
				});
				auto zoomOne = this->addToolBarElement("ofxCvGui::zoom_one", [this]() {
					this->zoom = Zoomed::ZoomOne;
				});

				auto mirrorToggle = this->addToolBarElement("ofxCvGui::mirror", [this]() {
					this->mirror ^= true;
				});
				
				this->toolBar->onDraw += [this, zoomOne, zoomFit, mirrorToggle](DrawArguments & args){
					ofPushStyle();
					{
						ofSetColor(150);
						ofSetLineWidth(2.0f);
						ofNoFill();
						switch (this->zoom) {
							case ZoomOne:
								ofDrawRectangle(zoomOne->getBounds());
								break;
							case ZoomFit:
								ofDrawRectangle(zoomFit->getBounds());
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
				zoomBox->addListenersToParent(this);
				zoomBox->setBounds(ofRectangle(100, 100, 150, 100));
				
				auto zoomBoxWeak = weak_ptr<Element>(zoomBox);
				
				zoomBox->onDraw += [this](DrawArguments & args) {
					ofRectangle zoomBoxBounds = args.localBounds;
					ofRectangle zoomSelection;
					
					zoomSelection.width = this->getWidth() / this->getImageWidth() * zoomBoxBounds.width;
					zoomSelection.height = this->getHeight() / this->getImageHeight() * zoomBoxBounds.height;
					ofVec2f zoomSelectionPos = -this->getScrollClamped() / ofVec2f(this->getImageWidth(), this->getImageHeight()) * ofVec2f(zoomBoxBounds.width, zoomBoxBounds.height);
					zoomSelection.x = zoomSelectionPos.x;
					zoomSelection.y = zoomSelectionPos.y;
					
					////
					//draw zoom box
					//
					
					//draw image
					{
						ofPushMatrix();
						{
							DrawImageArguments drawImageArgs(false
								, ofVec2f(this->getImageWidth(), this->getImageHeight())
								, ofVec2f(args.localBounds.width, args.localBounds.height)
								, ofVec2f(0, 0));
							ofScale(args.localBounds.width / this->getImageWidth(), args.localBounds.height / this->getImageHeight());
							this->applyMirror();
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
							this->scroll -= mouse.movement / ofVec2f(zoomBox->getWidth(), zoomBox->getHeight()) * ofVec2f(this->getImageWidth(), this->getImageHeight());
							this->scroll = this->getScrollClamped();
						}
					}
				};
			}
			
			this->onMouse.addListener([this](MouseArguments & args) {
				args.takeMousePress(this);
				if(args.isDragging(this)) {
					this->scroll += args.movement;
					this->scroll = this->getScrollClamped();
				}
			}, this, -1);
			
			this->onUpdate += [this, zoomBox](UpdateArguments &) {
				//update zoom box
				{
					auto zoomBoxEnabled = this->zoom == Zoomed::ZoomOne;
					
					//also check if there's no need to scale
					if(zoomBoxEnabled) {
						if(this->getWidth() >= this->getImageWidth() && this->getHeight() >= this->getImageHeight()) {
							zoomBoxEnabled = false;
						}
					}
					
					zoomBox->setEnabled(zoomBoxEnabled);
					if(zoomBoxEnabled) {
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
                scroll = this->getScrollClamped();
            }
        }
        
		//----------
		void BaseImage::applyMirror() const {
			if (this->mirror) {
				ofTranslate(this->getImageWidth(), 0.0f);
				ofScale(-1.0f, 1.0f, 1.0f);
			}
		}

		//----------
		void BaseImage::drawImage(DrawArguments& arguments) {
            if (this->zoom == ZoomFit) {
				DrawImageArguments args(false, ofVec2f(this->getImageWidth(), this->getImageHeight()), ofVec2f(this->getWidth(), this->getHeight()), ofVec2f(0,0));
				ofPushMatrix();
				{
					ofScale(this->getWidth() / this->getImageWidth(), this->getHeight() / this->getImageHeight());
					this->applyMirror();
					this->onDrawImage(args);
				}
				ofPopMatrix();
            } else {
                bool needsZoom = (this->getImageHeight() > this->getHeight() || this->getImageWidth() > this->getWidth());
                
                //calc translation
                ofVec2f scrollOffset;
                if (needsZoom) {
                    scrollOffset = this->getScrollClamped();
                } else {
                    scrollOffset = ofVec2f(this->getWidth() - this->getImageWidth(), this->getHeight() - this->getImageHeight()) / 2.0f;
                }
                
				ofPushMatrix();
				{
					ofTranslate(scrollOffset);
					DrawImageArguments args(true, ofVec2f(this->getImageWidth(), this->getImageHeight()), ofVec2f(this->getWidth(), this->getHeight()), scrollOffset);
					this->applyMirror();
					this->onDrawImage(args);
				}
				ofPopMatrix();
            }
		}
        
		//----------
        ofVec2f BaseImage::getScrollClamped() const {
            ofVec2f scroll = this->scroll;
            if (scroll.x > 0.0f || this->getImageWidth() < this->getWidth()) {
                scroll.x = 0.0f;
            }
            if (scroll.y > 0.0f || this->getImageHeight() < this->getHeight()) {
                scroll.y = 0.0f;
            }
            
            if (-scroll.x > this->getImageWidth() - this->getWidth()) {
                scroll.x = -(this->getImageWidth() - this->getWidth());
            }
            if (-scroll.y > this->getImageHeight() - this->getHeight()) {
                scroll.y = -(this->getImageHeight() - this->getHeight());
            }
            scroll.x = floor(scroll.x);
            scroll.y = floor(scroll.y);
            return scroll;
        }
	}
}