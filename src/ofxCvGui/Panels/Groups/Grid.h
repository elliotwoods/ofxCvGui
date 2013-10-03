#pragma once
#include "ofxCvGui/Panels/Groups/Base.h"
#include "ofMath.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			class Grid : public Groups::Base {
			public:
                Grid();
                virtual ~Grid();
				void setColsCount(int cols);
				void setHeights(vector<float>&);
				void setWidths(vector<float>&);
				PanelPtr const findScreen(const ofVec2f & xy) override;
			protected:
				void boundsChange(BoundsChangeArguments & arguments);
				float xCount, yCount;
				float panelWidth, panelHeight;
				bool xCountFixed;
				vector<float> heights, widths;
			};
		}
	}
}