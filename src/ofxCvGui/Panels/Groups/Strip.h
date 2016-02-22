#pragma once
#include "ofxCvGui/Panels/Groups/Base.h"
#include "ofMath.h"
#include <initializer_list>

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			class Strip : public Groups::Base {
			public:
				enum Direction {
					Horizontal = 0,
					Vertical = 1,
				};

				Strip(Direction = Direction::Horizontal);

				void setDirection(Direction);
				Direction getDirection() const;

				/// Usage: setSizes({ 50, 10, 100});
				/// Notes : size of -1 denotes auto-fill
				void setCellSizes(const initializer_list<int> &);
				
				void setUserCellSize(size_t index, int size);
				int getCellSize(size_t index) const;

				void setHandlesEnabled(bool);
				void resetUserCellSizes();
			protected:
				void boundsChange(BoundsChangeArguments &);
				void rebuildBorders();
				void layoutBorders();

				Direction direction;
				vector<int> programmaticCellSizes;
				vector<int> userCellSizes;
				vector<int> cachedCellSizes;

				const int minimumCellSize;
				const int borderSize;
				bool handlesEnabled = false;
				bool needsArrange = true;

				ElementGroupPtr borders;
			};

			shared_ptr<Strip> makeStrip(Strip::Direction direction = Strip::Direction::Horizontal);
		}
	}
}