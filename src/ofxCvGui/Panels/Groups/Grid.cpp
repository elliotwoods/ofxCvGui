#include "ofxCvGui/Panels/Groups/Grid.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			//----------
            Grid::Grid() {
				this->onBoundsChange.addListener([this] (BoundsChangeArguments& args) {
					this->boundsChange(args);
				}, this);
            }
            
            //----------
            Grid::~Grid() {
				onBoundsChange.removeListeners(this);
            }
            
			//----------
			const PanelPtr Grid::findScreen(const ofVec2f & xy) {
				unsigned int index = floor(xy.x / panelWidth) + xCount * floor(xy.y / panelHeight);
				if (index < this->elements.size()) {
					PanelPtr panel = this->elements[index];
					ofVec2f localXY = xy - ofVec2f(panel->getBounds().x, panel->getBounds().y);
					if (panel->findScreen(localXY) == PanelPtr())
						return panel;
				}
				return PanelPtr();
			}

			//----------
			void Grid::boundsChange(BoundsChangeArguments &) {
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

				for (auto it : this->elements) {
					iX = i % (int)xCount;
					iY = i / (int)xCount;
					bounds.x = iX * panelWidth;
					bounds.y = iY * panelHeight;

					it->setBounds(bounds);

					i++;
				}
			}
		}
	}
}