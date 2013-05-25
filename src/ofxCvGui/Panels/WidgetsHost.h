#pragma once
#include "ofxCvGui/Element.h"

#define OFX_UI_NO_XML
#include "ofxUI.h"

//you must include ofxUi.h before ofxCvGui.h in your code if you want to use this Panel

#ifdef OFXUI
namespace ofxCvGui {
	namespace Panels {
        namespace Utils {
            class WidgetsHost {
            public:
                WidgetsHost() {
                    this->widgets = 0;
                }
                
                ~WidgetsHost() {
                    this->clear();
                }
                
                void set(ofxUICanvas & widgets, Element * host) {
                    this->set(&widgets, host);
                }
                
                void set(ofxUICanvas * widgets, Element * host) {
                    this->clear();
                    
                    this->widgets = widgets;
                    this->host = host;
                    
                    ofAddListener(this->host->onUpdate, this, &WidgetsHost::update);
                    ofAddListener(this->host->onDraw, this, &WidgetsHost::draw);
                    ofAddListener(this->host->onMouseAction, this, &WidgetsHost::mouseAction);
                    ofAddListener(this->host->onBoundsChange, this, &WidgetsHost::boundsChange);
                    
                    this->widgets->init(this->host->getWidth(), this->host->getHeight());
                    //this->widgets->setFont("ofxCvGui/fonts/swisop3.ttf");
                    this->widgets->disableAppEventCallbacks();
                    this->widgets->disableMouseEventCallbacks();
                    this->widgets->enableKeyEventCallbacks();
                }
                
                void clear() {
                    if (this->widgets != 0) {
                        ofRemoveListener(host->onUpdate, this, &WidgetsHost::update);
                        ofRemoveListener(host->onDraw, this, &WidgetsHost::draw);
                        ofRemoveListener(host->onMouseAction, this, &WidgetsHost::mouseAction);
                        ofRemoveListener(host->onBoundsChange, this, &WidgetsHost::boundsChange);

                        this->widgets = 0;
                    }
                }
                
                bool hasWidgets() {
                    return this->widgets != 0;
                }
        
            protected:
                ofxUICanvas * widgets;
                Element * host;
                
                //---------
                void update(UpdateArguments&) {
                    if (this->hasWidgets()) {
                        this->widgets->update();
                    }
                }
                
                //---------
                void draw(DrawArguments& arguments) {
                    if (this->hasWidgets()) {
                        ofPushView();
                        ofViewport(host->getBounds());
                        ofSetupScreenOrtho(this->host->getWidth(), this->host->getHeight());
                        this->widgets->draw();
                        ofPopView();
                    }
                }
                
                //---------
                void mouseAction(MouseArguments& mouse) {
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
                
                //---------
                void boundsChange(ofRectangle & bounds) {
                    if (this->hasWidgets()) {
                        this->widgets->windowResized(bounds.x, bounds.y);
                    }
                }
            };
        }
    }
}

#endif //OFXUI