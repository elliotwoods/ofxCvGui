#include "ElementCanvas.h"

namespace ofxCvGui {
	namespace Panels {
		//---------
		ElementCanvas::ElementCanvas() {
			this->dragging = false;

			this->canvasElements = shared_ptr<ElementGroup>(new ElementGroup);
			this->fixedElements = shared_ptr<ElementGroup>(new ElementGroup);

			this->onUpdate += [this](ofxCvGui::UpdateArguments &) {
				this->update();

				this->canvasElements->update();
				this->fixedElements->update();
			};

			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				this->canvasElements->draw(args);
				this->fixedElements->draw(args);
			};

			this->onMouse += [this](ofxCvGui::MouseArguments & args) {
				this->fixedElements->mouseAction(args);
				if (args.mightStillBeUseful()) {
					this->canvasElements->mouseAction(args);
				}
				if (args.isLocalPressed() && !args.isTaken()) {
					args.take();
					this->dragging = true;
				}
				else if(args.action == MouseArguments::Action::Dragged) {
					this->scrollPosition -= args.movement;
				}
				else {
					this->dragging = false;
				}
			};

			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments & args) {
				this->fixedElements->setBounds(args.localBounds);
			};
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
				auto elementBounds = element->getBounds();
				if (elementBounds.x < topLeft.x) {
					topLeft.x = elementBounds.x;
				}
				if (elementBounds.y < topLeft.y) {
					topLeft.y = elementBounds.y;
				}
				if (elementBounds.x > bottomRight.x) {
					bottomRight.x = elementBounds.x;
				}
				if (elementBounds.y > bottomRight.y) {
					bottomRight.y = elementBounds.y;
				}
			}

			this->canvasExtents = ofRectangle(topLeft, bottomRight);

			auto canvasBounds = this->canvasExtents;
			canvasBounds.x -= this->scrollPosition.x;
			canvasBounds.y -= this->scrollPosition.y;
			this->canvasElements->setBounds(canvasBounds);

			auto intersectionViewAndCanvas = canvasBounds.getIntersection(this->getLocalBounds());
			if (intersectionViewAndCanvas.width < this->getWidth() / 3.0f || intersectionViewAndCanvas.height < this->getHeight() / 3.0f) {
				this->scrollPosition += 0.005f * (this->canvasExtents.getCenter() - this->scrollPosition);
			}
		}
	}
}