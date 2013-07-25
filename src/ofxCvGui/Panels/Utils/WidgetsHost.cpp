#ifndef OFXCVGUI_DONT_USE_OFXUI
#include "WidgetsHost.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Utils {
			//----------
			WidgetsHost::WidgetsHost() {
				this->widgets = 0;
			}
			
			//----------
			WidgetsHost::~WidgetsHost() {
				this->clear();
			}
			
			//----------
			void WidgetsHost::set(ofxUICanvas & widgets, Element * host) {
                    this->set(&widgets, host);
			}
			
			//----------
			void WidgetsHost::set(ofxUICanvas * widgets, Element * host) {
				this->clear();
				
				this->widgets = widgets;
				this->host = host;
				
				this->host->onUpdate.addListener([this] (UpdateArguments& args) {
					this->update(args);
				}, this);
				this->host->onDraw.addListener([this] (DrawArguments&) {
					this->draw();
				}, this);
				this->host->onMouse.addListener([this] (MouseArguments& args) {
					this->mouseAction(args);
				}, this);
				this->host->onBoundsChange.addListener([this] (BoundsChangeArguments& args) {
					this->resize(args.bounds);
				}, this);
				
				this->widgets->init(this->host->getWidth(), this->host->getHeight());
				//this->widgets->setFont("ofxCvGui/fonts/swisop3.ttf");
				this->widgets->disableAppEventCallbacks();
				this->widgets->disableMouseEventCallbacks();
				this->widgets->enableKeyEventCallbacks();
			}
			
			//----------
			void WidgetsHost::clear() {
				if (this->widgets != 0) {
					host->onUpdate.removeListeners(this);
					host->onDraw.removeListeners(this);
					host->onMouse.removeListeners(this);
					host->onBoundsChange.removeListeners(this);
					this->widgets = 0;
				}
			}
			
			//----------
			bool WidgetsHost::hasWidgets() {
				return this->widgets != 0;
			}
			
			//----------
			void WidgetsHost::update(UpdateArguments &) {
				if (this->hasWidgets()) {
					this->widgets->update();
				}
			}
			
			//----------
			void WidgetsHost::draw() {
				if (this->hasWidgets()) {
					ofPushView();
					ofViewport(host->getBounds());
					ofSetupScreenOrtho(this->host->getWidth(), this->host->getHeight());
					this->widgets->draw();
					ofPopView();
				}
			}
			
			//----------
			void WidgetsHost::mouseAction(MouseArguments& mouse) {
				if (this->hasWidgets()) {
					switch (mouse.action) {
						case MouseArguments::Pressed:
							this->widgets->mousePressed(mouse.local.x, mouse.local.y, mouse.button);
							break;
						case MouseArguments::Released:
							this->widgets->mouseReleased(mouse.local.x, mouse.local.y, mouse.button);
							break;
						case MouseArguments::Dragged:
							//hack because there seems to be an issue in ofxUI
							this->widgets->mouseDragged(mouse.local.x, mouse.local.y, mouse.button);
							break;
						case MouseArguments::Moved:
							this->widgets->mouseMoved(mouse.local.x, mouse.local.y);
							break;
					}
				}
			}

			//----------
			void WidgetsHost::resize(const ofRectangle & bounds) {
				if (this->hasWidgets()) {
					this->widgets->windowResized(bounds.x, bounds.y);
				}
			}
		}
	}
}
#endif
