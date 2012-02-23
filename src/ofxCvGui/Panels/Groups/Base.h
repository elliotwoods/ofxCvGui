#pragma once
#include "ofxCvGui/ElementGroup.h"
#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			class Base : public ElementGroup_<Panels::Base> {
			protected:
				void drawPanel(const DrawArguments& arguments) {
					this->drawSet(arguments);
				}
			};
		}
	}
	typedef ofPtr<Panels::Groups::Base> PanelGroupPtr;
}