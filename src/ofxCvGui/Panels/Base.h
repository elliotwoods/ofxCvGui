#pragma once
#include "../Element.h"
#include "./Utils/WidgetsHost.h"
#include "ofRectangle.h"

namespace ofxCvGui {
	namespace Panels {
		class Base : public Element {
		public:
			Base();
			virtual const shared_ptr<Panels::Base> findScreen(const ofVec2f & xy, ofRectangle & currentPanelBounds) {
				return shared_ptr<Panels::Base>();
			}
			
			ofxLiquidEvent<FilesDraggedArguments> ofFilesDragged;

#ifdef OFXUI
            void setWidgets(ofxUICanvas & gui) {
                this->widgets.set(gui, this);
				gui.setPosition(20, 80);
				gui.setHeight(1024);
				gui.setColorBack(ofColor(0, 150));
            }
            void clearWidgets() {
                this->widgets.clear();
            }
#endif
		protected:
#ifdef OFXUI
            Utils::WidgetsHost widgets;
#endif
		};
	}
	typedef shared_ptr<Panels::Base> PanelPtr;
}