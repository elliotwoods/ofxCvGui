#pragma once
#include "ofxCvGui/ElementGroup.h"
#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			class Base : public ElementGroup_<Panels::Base> {
			public:
				virtual ~Base() { }
			};
		}
	}
	typedef ofPtr<Panels::Groups::Base> PanelGroupPtr;
}