#include "ofxCvGui/Groups/PanelGroup.h"

namespace ofxCvGui {
	namespace Groups {
		//----------
		void PanelGroup::boundsChange() {
			float count = this->elements.size();
			float xCount = ceil(sqrt(count));
			float yCount = ceil(count / xCount);

			float width = this->bounds.width / xCount;
			float height = this->bounds.height / yCount;

			ElementIterator it;
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