#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Panels {
		namespace Groups {
			//----------
			const PanelPtr Base::findScreen(const glm::vec2 & xy, ofRectangle & currentBounds) {
				for (auto panel : this->elements) {
					if (panel->getBounds().inside(xy)) {
						auto localXY = xy - glm::vec2(panel->getBounds().x, panel->getBounds().y);
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