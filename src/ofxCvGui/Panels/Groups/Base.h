#pragma once
#include "ofxCvGui/ElementGroup.h"
#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			class Base : public ElementGroup_<Panels::Base> {
			public:
			virtual const PanelPtr findScreen(const ofVec2f & xy) { return PanelPtr(); }
			protected:
				void drawPanel(const DrawArguments& arguments) {
					this->drawSet(arguments);
				}
			};
		}
	}
	typedef ofPtr<Panels::Groups::Base> PanelGroupPtr;
}