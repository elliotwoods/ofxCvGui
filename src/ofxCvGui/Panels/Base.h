#pragma once
#include "ofxCvGui/Element.h"
#include "ofxCvGui/Panels/Utils/WidgetsHost.h"

namespace ofxCvGui {
	namespace Panels {
		class Base : public Element {
		public:
            Base();
            virtual ~Base();
			virtual const ofPtr<Panels::Base> findScreen(const ofVec2f & xy) { return ofPtr<Panels::Base>(); }
			
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
	typedef ofPtr<Panels::Base> PanelPtr;
}