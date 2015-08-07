#include "ElementCanvas.h"
#include "../Widgets/Slider.h"

#include "ofAppRunner.h"

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
				if (args.isDragging(this)) {
					this->scrollPosition -= args.movement;
				}
			};
			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments & args) {
				const ofVec2f bottomRight = this->getLocalBounds().getBottomRight();
				this->zoomControl->setBounds(ofRectangle(bottomRight - ofVec2f(210.0f, 50.0f), bottomRight - ofVec2f(10.0f, 10.0f)));
			};

			canvasElements->onZoomChange += [this](ofxCvGui::ZoomChangeArguments & args) {
				const auto zoomRatio = args.newZoom / args.oldZoom;

				// MP = midPoint
				// TL = top left

				auto MP = ofVec2f(this->getWidth() / 2.0f, this->getHeight() / 2.0f);
				auto TL = -this->getScrollPosition();
				auto MP_TL = TL - MP;
				auto MP_TL_new = MP_TL * zoomRatio;
				

				this->setScrollPosition(-MP - MP_TL_new);
			};

			this->canvasElements->addListenersToParent(this);
			this->fixedElements->addListenersToParent(this, true);

			this->zoom.set("Zoom", 0, -3, 1);
			this->zoomControl = Widgets::Slider::make(this->zoom);
			this->zoomControl->setBounds(ofRectangle(10, 10, 100, 50));
			this->zoomControl->addListenersToParent(this->fixedElements);
			this->zoom.addListener(this, &ElementCanvas::callbackZoomChange);
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
		void ElementCanvas::setScrollPosition(const ofVec2f & scrollPosition) {
			this->scrollPosition = scrollPosition;
		}

		//---------
		const ofVec2f & ElementCanvas::getScrollPosition() const {
			return this->scrollPosition;
		}

		//---------
		const ofRectangle & ElementCanvas::getCanvasExtents() const {
			return this->canvasExtents;
		}

		//---------
		void ElementCanvas::update() {
			auto thisBounds = this->getBounds();



			//--
			//Calculate canvas extents
			//--
			//
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
			//
			//--



			//--
			//Set canvasBounds to size of extents and position of scroll
			//--
			//
			auto canvasBounds = this->canvasExtents;
			canvasBounds.x -= this->scrollPosition.x;
			canvasBounds.y -= this->scrollPosition.y;
			if (this->canvasElements->getBounds() != canvasBounds) {
				this->canvasElements->setBounds(canvasBounds);
			}
			//
			//--
			


			//--
			//Rubber band the scrollPosition back if off-edge
			//--
			//
			//NOTE : should really be rubberbanding to edge not center
			auto zoomedCanvasBounds = canvasElements->getBoundsInParent();
			auto localBounds = this->getLocalBounds();
			auto intersectionViewAndCanvas = zoomedCanvasBounds.getIntersection(localBounds);
			auto center = zoomedCanvasBounds.getCenter();
			if (intersectionViewAndCanvas.width < this->getWidth() / 3.0f) {
				this->scrollPosition.x += 0.5f * (center.x - this->getWidth() / 2.0f) * ofGetLastFrameTime();
			}
			if (intersectionViewAndCanvas.height < this->getHeight() / 3.0f) {
				this->scrollPosition.y += 0.5f * (center.y - this->getHeight() / 2.0f) * ofGetLastFrameTime();
			}
			//
			//--
		}

		//----------
		void ElementCanvas::callbackZoomChange(float & zoom) {
			this->canvasElements->setZoom(pow(2, zoom));
		}
	}
}