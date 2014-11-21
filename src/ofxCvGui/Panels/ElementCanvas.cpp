#include "ElementCanvas.h"

namespace ofxCvGui {
	namespace Panels {
		//---------
		ElementCanvas::ElementCanvas() {
			this->canvasElements = shared_ptr<ElementGroup>(new ElementGroup);
			this->fixedElements = shared_ptr<ElementGroup>(new ElementGroup);

			this->onUpdate += [this](ofxCvGui::UpdateArguments &) {
				this->update();
			};

			this->onMouse += [this](ofxCvGui::MouseArguments & args) {
				args.takeMousePress(this);
				if(args.isDragging(this)) {
					this->scrollPosition -= args.movement;
				}
			};

			this->canvasElements->addListenersToParent(this);
			this->fixedElements->addListenersToParent(this, true);
		}

		//---------
		ElementGroupPtr ElementCanvas::getCanvasElementGroup() {
			return this->canvasElements;
		}

		//---------
		ElementGroupPtr ElementCanvas::getFixedElementGroup() {
			return this->fixedElements;
		}

		//---------
		void ElementCanvas::update() {
			auto thisBounds = this->getBounds();

			ofVec2f topLeft(0, 0);
			ofVec2f bottomRight(thisBounds.getWidth(), thisBounds.getHeight());

			for (auto element : this->canvasElements->getElements()) {
				auto elementBottomRight = element->getBounds().getBottomRight();
				if (elementBottomRight.x > bottomRight.x) {
					bottomRight.x = elementBottomRight.x + 10;
				}
				if (elementBottomRight.y > bottomRight.y) {
					bottomRight.y = elementBottomRight.y + 10;
				}
			}

			this->canvasExtents = ofRectangle(topLeft, bottomRight);

			auto canvasBounds = this->canvasExtents;
			canvasBounds.x -= this->scrollPosition.x;
			canvasBounds.y -= this->scrollPosition.y;
			if (this->canvasElements->getBounds() != canvasBounds) {
				this->canvasElements->setBounds(canvasBounds);
			}

			auto intersectionViewAndCanvas = canvasBounds.getIntersection(this->getLocalBounds());
			if (intersectionViewAndCanvas.width < this->getWidth() / 3.0f || intersectionViewAndCanvas.height < this->getHeight() / 3.0f) {
				this->scrollPosition += 0.005f * (this->canvasExtents.getCenter() - this->scrollPosition);
			}
		}
	}
}