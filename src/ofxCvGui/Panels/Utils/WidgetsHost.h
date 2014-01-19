#pragma once
#include "../../Element.h"

#ifndef OFXCVGUI_DONT_USE_OFXUI
#define OFX_UI_NO_XML
#include "ofxUI/src/ofxUI.h"

namespace ofxCvGui {
	namespace Panels {
        namespace Utils {
            class WidgetsHost {
            public:
                WidgetsHost();
                virtual ~WidgetsHost();
                
                void set(ofxUICanvas & widgets, Element * host);
                void set(ofxUICanvas * widgets, Element * host);
                void clear();
                bool hasWidgets();
        
            protected:
                ofxUICanvas * widgets;
                Element * host;
                
                void update(UpdateArguments&);
                void draw();
                void mouseAction(MouseArguments& mouse);
                void resize(const ofRectangle & bounds);
            };
        }
    }
}

#endif