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
				void setHeights(vector<float>&); ///< Note : You must call arrange() or setBounds(..) after calling this
				void setWidths(vector<float>&); ///< Note : You must call arrange() or setBounds(..) after calling this
				PanelPtr const findScreen(const ofVec2f & xy, ofRectangle & currentBounds) override;
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