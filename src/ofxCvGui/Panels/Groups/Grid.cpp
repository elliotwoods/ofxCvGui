#include "ofxCvGui/Panels/Groups/Grid.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			//----------
			void Grid::boundsChange() {
				float count = this->elements.size();
				float xCount = ceil(sqrt(count));
				float yCount = ceil(count / xCount);

				float width = this->bounds.width / xCount;
				float height = this->bounds.height / yCount;

				PanelIterator it;
				int i=0;
				int iX, iY;
				ofRectangle bounds;
				bounds.width = width;
				bounds.height = height;

				for (it = elements.begin(); it != elements.end(); it++, i++) {
					iX = i % (int)xCount;
					iY = i / (int)xCount;
					bounds.x = iX * width;
					bounds.y = iY * height;

					(**it).setBounds(bounds);
				}
			}
		}
	}
}