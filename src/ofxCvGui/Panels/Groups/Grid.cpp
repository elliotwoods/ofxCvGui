#include "ofxCvGui/Panels/Groups/Grid.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			//----------
            Grid::Grid() {
				this->xCountFixed = false;

				//some unecessary but tidy initialisations
				this->xCount = 0;
				this->yCount = 0;
				this->panelWidth = 0.0f;
				this->panelHeight = 0.0f;

				this->onBoundsChange.addListener([this] (BoundsChangeArguments& args) {
					this->boundsChange(args);
				}, this);
            }
            
            //----------
            Grid::~Grid() {
				onBoundsChange.removeListeners(this);
            }
            
			//----------
			void Grid::setColsCount(int cols) {
				this->xCountFixed = true;
				this->xCount = cols;

				BoundsChangeArguments args(this->getBounds()); // these aren't actually used
				this->boundsChange(args);
			}

			//----------
			void Grid::setHeights(vector<float>& heights) {
				this->heights = heights;
			}

			//----------
			void Grid::setWidths(vector<float>& widths) {
				this->widths = widths;
			}

			//----------
			const PanelPtr Grid::findScreen(const ofVec2f & xy, ofRectangle & currentBounds) {
				for(auto panel : this->elements) {
					if (panel->getBounds().inside(xy)) {
						ofVec2f localXY = xy - ofVec2f(panel->getBounds().x, panel->getBounds().y);
						auto searchCurrentBounds = panel->getBounds();
						searchCurrentBounds.x += currentBounds.x;
						searchCurrentBounds.y += currentBounds.y;
						auto innerPanel = panel->findScreen(localXY, searchCurrentBounds);
						currentBounds = searchCurrentBounds;
						if (innerPanel == PanelPtr()) {
							return panel;
						} else {
							return innerPanel;
						}
					}
				}
				return PanelPtr();
			}

			//----------
			void Grid::boundsChange(BoundsChangeArguments &) {
				float count = this->elements.size();
				if (!this->xCountFixed) {
					xCount = ceil(sqrt(count));
				}
				yCount = ceil(count / xCount);

				panelWidth = this->bounds.width / xCount;
				panelHeight = this->bounds.height / yCount;

				PanelIterator it;
				int i=0;
				ofRectangle bounds;
				bounds.x = 0;
				bounds.width = panelWidth;
				bounds.height = panelHeight;

				float y = 0;
				int row = 0;
				int col = 0;
				if (this->heights.size() > 0) {
					bounds.height = heights[0];
				}
				for (auto it : this->elements) {
					if (col < this->widths.size()) {
						bounds.width = widths[col];
					} else {
						bounds.x = col * panelWidth;
					}
						
					//stop strange drawing artefacts when drawing on pixel boundaries
					//e.g. text, lines
					bounds.x = floor(bounds.x);
					bounds.y = floor(y);
					bounds.width = floor(bounds.width);
					bounds.height = floor(bounds.height);

					it->setBounds(bounds);

					if (col < this->widths.size()) {
						bounds.x += this->widths[col];
					}

					col++;
					if (col >= xCount) {
						row++;
						col = 0;
						y += bounds.height;
						bounds.x = 0;

						if (this->heights.size() > row) {
							bounds.height = this->heights[row];
						}
					}
					i++;
				}
			}
		}
	}
}