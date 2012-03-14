#include "ofxCvGui/Panels/Groups/Grid.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			//----------
			const PanelPtr Grid::findScreen(const ofVec2f & xy) {
				unsigned int index = floor(xy.x / panelWidth) + xCount * floor(xy.y / panelHeight);
				if (index < this->elements.size()) {
					PanelPtr element = this->elements[index];
					ofVec2f localXY = xy - ofVec2f(element->getBounds().x, element->getBounds().y);
					if (element->findScreen(localXY) == PanelPtr())
						return element;
				}
				return PanelPtr();
			}

			//----------
			void Grid::boundsChange() {
				float count = this->elements.size();
				xCount = ceil(sqrt(count));
				yCount = ceil(count / xCount);

				panelWidth = this->bounds.width / xCount;
				panelHeight = this->bounds.height / yCount;

				PanelIterator it;
				int i=0;
				int iX, iY;
				ofRectangle bounds;
				bounds.width = panelWidth;
				bounds.height = panelHeight;

				for (it = elements.begin(); it != elements.end(); it++, i++) {
					iX = i % (int)xCount;
					iY = i / (int)xCount;
					bounds.x = iX * panelWidth;
					bounds.y = iY * panelHeight;

					(*it)->setBounds(bounds);
				}
			}
		}
	}
}