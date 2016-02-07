#include "Base.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			//----------
			const PanelPtr Base::findScreen(const ofVec2f & xy, ofRectangle & currentBounds) {
				for (auto panel : this->elements) {
					if (panel->getBounds().inside(xy)) {
						ofVec2f localXY = xy - ofVec2f(panel->getBounds().x, panel->getBounds().y);
						auto searchCurrentBounds = panel->getBounds();
						searchCurrentBounds.x += currentBounds.x;
						searchCurrentBounds.y += currentBounds.y;
						auto innerPanel = panel->findScreen(localXY, searchCurrentBounds);
						currentBounds = searchCurrentBounds;
						if (innerPanel == PanelPtr()) {
							return panel;
						}
						else {
							return innerPanel;
						}
					}
				}
				return PanelPtr();
			}

		}
	}
}