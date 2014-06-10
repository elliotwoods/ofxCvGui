#include "ofxCvGui/Panels/BaseImage.h"
#include "ofxAssets.h"

using namespace ofxAssets;

namespace ofxCvGui {
	namespace Panels {
        //----------
        ofVec2f BaseImage::scroll = ofVec2f();
        
        //----------
		BaseImage::DrawCroppedArguments::DrawCroppedArguments(bool zoomed, const ofVec2f & size, const ofVec2f & offsetCropped) {
			this->zoomed = zoomed;
			this->size = size;
			this->offsetCropped = offsetCropped;
		}

        //----------
		BaseImage::Zoomed BaseImage::getZoomed() const {
			return this->zoom;
		}

        //----------
        BaseImage::BaseImage() {
			this->onDraw.addListener([this] (DrawArguments & args) {
				this->drawImage(args);
			}, 0, this);

			this->onMouse.addListener([this] (MouseArguments & args) {
				this->mouseAction(args);
			}, this);

            this->zoom = ZoomFit;
            this->dragSelection = DragNone;
            this->buttonFitBounds = ofRectangle(20, 20, 30, 30);
            this->buttonOneBounds = ofRectangle(60, 20, 30, 30);
            this->zoomBox = ofRectangle(100, 100, 150, 100);
        }
        
        //----------
        BaseImage::~BaseImage() {
			this->onMouse.removeListeners(this);
        }
        
		//----------
        void BaseImage::mouseAction(MouseArguments& mouse) {
            if (mouse.action == MouseArguments::Dragged && this->zoom != ZoomFit) {
                if (this->dragSelection == DragImage) {
                    this->scroll += mouse.movement;
                    this->scroll = this->getScrollClamped();
                } else if (this->dragSelection == DragZoomBox) {
                    this->scroll -= mouse.movement / ofVec2f(zoomBox.width, zoomBox.height) * ofVec2f(this->getImageWidth(), this->getImageHeight());
                    this->scroll = this->getScrollClamped();
                }
            }
            
            if (mouse.action == MouseArguments::Pressed && mouse.checkCurrentPanel(this)) {
                if (this->buttonFitBounds.inside(mouse.local)) {
                    this->zoom = ZoomFit;
                } else if (this->buttonOneBounds.inside(mouse.local)) {
                    this->zoom = ZoomOne;
                } else if (this->zoomBox.inside(mouse.local)) {
                    this->dragSelection = DragZoomBox;
                } else {
                    this->dragSelection = DragImage;
                }
            }
            
            if (mouse.action == MouseArguments::Released) {
                this->dragSelection = DragNone;
            }
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
		void BaseImage::drawImage(DrawArguments& arguments) {
            if (this->zoom == ZoomFit) {
                this->drawImage(this->getWidth(), this->getHeight());
				DrawCroppedArguments args(false, ofVec2f(this->getImageWidth(), this->getImageHeight()), ofVec2f(0,0));
				ofPushMatrix();
				ofScale(this->getWidth() / this->getImageWidth(), this->getHeight() / this->getImageHeight());
				this->onDrawCropped(args);
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
				ofTranslate(scrollOffset);
                this->drawImage(this->getImageWidth(), this->getImageHeight());
				DrawCroppedArguments args(true, ofVec2f(this->getImageWidth(), this->getImageHeight()), scrollOffset);
				this->onDrawCropped(args);
				ofPopMatrix();

                //only draw zoom box if we need to
                if (needsZoom && arguments.chromeEnabled) {
                    //update zoom box
                    zoomBox.height = this->getImageHeight() / this->getImageWidth() * zoomBox.width;
                    zoomBox.x = this->getWidth() - zoomBox.width - 20;
                    zoomBox.y = this->getHeight() - zoomBox.height - 20;
                    
                    //update zoom selection
                    zoomSelection.width = this->getWidth() / this->getImageWidth() * zoomBox.width;
                    zoomSelection.height = this->getHeight() / this->getImageHeight() * zoomBox.height;
                    ofVec2f zoomSelectionPos = -this->getScrollClamped() / ofVec2f(this->getImageWidth(), this->getImageHeight()) * ofVec2f(zoomBox.width, zoomBox.height);
                    zoomSelection.x = zoomSelectionPos.x + zoomBox.x;
                    zoomSelection.y = zoomSelectionPos.y + zoomBox.y;
                    
                    ////
                    //draw zoom box
                    //
                    
                    //draw image
                    ofPushMatrix();
                    ofTranslate(zoomBox.x, zoomBox.y);
                    this->drawImage(zoomBox.width, zoomBox.height);
                    ofPopMatrix();
                    
                    ofPushStyle();
                    
                    //draw outer
                    ofSetColor(150);
                    ofNoFill();
                    ofSetLineWidth(2.0f);
                    ofRect(zoomBox);
                    
                    //draw inner
                    ofEnableAlphaBlending();
                    ofFill();
                    ofSetLineWidth(0.0f);
                    ofSetColor(255, 255, 255, 100);
                    ofRect(zoomSelection);
                    
                    ofPopStyle();
                    
                    //
                    ////
                }
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
		
		//----------
		void BaseImage::drawTitle() {
			image("ofxCvGui::zoom_fit").draw(buttonFitBounds);
			image("ofxCvGui::zoom_one").draw(buttonOneBounds);
			Utils::drawText(this->caption, 100, 20, true, 30);
			
			ofPushStyle();
			ofSetColor(150);
			ofSetLineWidth(2.0f);
			ofNoFill();
			ofRect(this->zoom == ZoomFit ? buttonFitBounds : buttonOneBounds);
			ofPopStyle();
		}
	}
}