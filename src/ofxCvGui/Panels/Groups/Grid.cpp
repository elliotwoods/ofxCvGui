#include "pch_ofxCvGui.h"

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
			void Grid::setHeights(const vector<float> & heights) {
				this->heights = heights;
			}

			//----------
			void Grid::setWidths(const vector<float> & widths) {
				this->widths = widths;
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
				size_t row = 0;
				size_t col = 0;
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

			//----------
			shared_ptr<Grid> makeGrid() {
				return make_shared<Grid>();
			}
		}
	}
}