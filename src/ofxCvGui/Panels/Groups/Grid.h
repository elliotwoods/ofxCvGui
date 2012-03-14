#pragma once
#include "ofxCvGui/Panels/Groups/Base.h"
#include "ofMath.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			class Grid : public Groups::Base {
			public:
				const PanelPtr findScreen(const ofVec2f & xy);
			protected:
				void boundsChange();
				float xCount, yCount;
				float panelWidth, panelHeight;
			};
		}
	}
}