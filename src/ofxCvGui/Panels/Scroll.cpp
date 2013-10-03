#include "Scroll.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Scroll::Scroll() {
			this->elements = ElementGroupPtr(new ElementGroup());
			this->onUpdate += [this] (UpdateArguments&) { this->update(); };
			this->onDraw += [this] (DrawArguments& args) { this->draw(args); };
			this->onMouse += [this] (MouseArguments& args) { this->mouse(args); };
			this->onBoundsChange += [this] (BoundsChangeArguments&) { this->arrange(); };
			this->position = 0.0f;
			this->length = 0.0f;
			this->onScrollBar = false;
		}

		//----------
		void Scroll::add(ElementPtr element) {
			this->elements->add(element);
			this->arrange();
		}

		//----------
		ElementGroupPtr Scroll::getGroup() {
			return this->elements;
		}

		//----------
		void Scroll::setScroll(float position) {
			this->position = position;
			this->elements->setPosition(ofVec2f(0, -position));
		}

		//----------
		void Scroll::update() {
			if (this->position < 0.0f) {
				this->setScroll(this->position * 0.9f);
			} else if (this->position > this->length - this->getHeight()) {
				float y = (this->length - this->getHeight()) - this->position;
				y *= 0.9f;
				this->setScroll(y + this->position);
			}
		}

		//----------
		void Scroll::draw(DrawArguments& args) {
			ofPushView();
			ofViewport(args.parentBounds);
			ofSetupScreen();
			ofSetDrawBitmapMode(OF_BITMAPMODE_SIMPLE);
			this->elements->draw(args);

			float barLength = this->getBarLength();
			if (barLength > this->getHeight() - 2 * OFXCVGUI_SCROLL_AREA_WIDTH) {
				return; // no need for scroll
			}

			float barPosition = this->getBarY();

			ofPushStyle();
			ofSetColor(255);
			float x = this->getWidth() - OFXCVGUI_SCROLL_AREA_WIDTH / 2.0f;
			ofSetLineWidth(0.0f);
			ofCircle(x, barPosition, OFXCVGUI_SCROLL_BAR_WIDTH / 2.0f);
			ofCircle(x, barPosition + barLength, OFXCVGUI_SCROLL_BAR_WIDTH / 2.0f);
			ofSetLineWidth(OFXCVGUI_SCROLL_BAR_WIDTH);
			ofLine(x, barPosition, x, barPosition + barLength);
			ofPopStyle();
		
			ofPopView();
		}

		//----------
		void Scroll::mouse(MouseArguments& args) {
			if (args.action == MouseArguments::Pressed) {
				this->onScrollBar = args.local.x > this->getWidth() - OFXCVGUI_SCROLL_AREA_WIDTH;
			} else if (args.action == MouseArguments::Dragged && this->getMouseState() == Element::Dragging) {
				if (this->onScrollBar) {
					const float range = this->length - this->getHeight();
					const float spareScrollSpace = this->getHeight() - this->getBarLength();
					this->setScroll(this->position + args.movement.y * range / spareScrollSpace);
				} else {
					this->setScroll(this->position - args.movement.y);
				}
			}
			this->elements->mouseAction(args);
		}

		//----------
		void Scroll::arrange() {
			float y = 0;
			for(auto element : this->elements->getElements()) {
				auto elementBounds = element->getBounds();
				elementBounds.y = y;
				elementBounds.width = this->getWidth() - OFXCVGUI_SCROLL_AREA_WIDTH;
				element->setBounds(elementBounds);
				y += elementBounds.height + OFXCVGUI_SCROLL_SPACING;
			}
			this->length = y;
		}

		//----------
		float Scroll::getBarLength() const {
			float barPossibleLength = this->getHeight() - OFXCVGUI_SCROLL_AREA_WIDTH * 2.0f;
			float pct = this->getHeight() / this->length; //fraction visible
			return pct * barPossibleLength;
		}

		//----------
		float Scroll::getBarY() const {
			float barPossibleLength = this->getHeight() - OFXCVGUI_SCROLL_AREA_WIDTH * 2.0f;
			float scrollPct = this->position / (this->length - this->getHeight()); //fraction scrolled
			return OFXCVGUI_SCROLL_AREA_WIDTH + (barPossibleLength - this->getBarLength()) * scrollPct;
		}
	}
}