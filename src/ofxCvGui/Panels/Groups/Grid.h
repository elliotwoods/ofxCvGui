#pragma once
#include "ofxCvGui/ElementGroup.h"
#include "ofMath.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			class Grid : public PanelGroup {
			protected:
				void boundsChange();
			};
		}
	}
}