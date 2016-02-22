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
				void setHeights(const vector<float> &); ///< Note : You must call arrange() or setBounds(..) after calling this
				void setWidths(const vector<float> &); ///< Note : You must call arrange() or setBounds(..) after calling this
			protected:
				void boundsChange(BoundsChangeArguments & arguments);
				float xCount, yCount;
				float panelWidth, panelHeight;
				bool xCountFixed;
				vector<float> heights, widths;
			};

			shared_ptr<Grid> makeGrid();
		}
	}
}