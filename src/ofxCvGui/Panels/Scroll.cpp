#include "Scroll.h"

#define OFXCVGUI_SCROLL_SPACING 2.0f
#define OFXCVGUI_SCROLL_AREA_WIDTH 15.0f
#define OFXCVGUI_SCROLL_BAR_WIDTH 3.0f

namespace ofxCvGui {
	namespace Panels {
		//----------
		Scroll::Scroll() {
			this->elements = ElementGroupPtr(new ElementGroup());
			this->onUpdate += [this] (UpdateArguments & args) { this->update();	};
			this->onDraw += [this] (DrawArguments & args) { this->draw(args); };
			this->onMouse += [this] (MouseArguments & args) { this->mouse(args); };
			this->onKeyboard += [this] (KeyboardArguments & args) { this->keyboard(args); };
			this->onBoundsChange += [this] (BoundsChangeArguments & args) { this->arrange(args); };
			this->position = 0.0f;
			this->length = 0.0f;
			this->onScrollBar = false;
		}

		//----------
		void Scroll::add(ElementPtr element) {
			this->elements->add(element);
			this->arrange(BoundsChangeArguments(this->getBounds()));
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
			this->elements->update();
			if (this->localMouseState == LocalMouseState::Waiting) {
				if (this->position < 0.0f) {
					this->setScroll(this->position * 0.9f);
				} else {
					float overflow = this->length - this->getHeight();
					if (overflow < 0 && this->position > 0.0f) {
						//less content than screen space
						this->setScroll(this->position * 0.9f);
					} else {
						//more content than screen space
						if (this->position > overflow) {
							float position = this->position - overflow;
							position *= 0.9f;
							this->setScroll(position + overflow);
						}
					}
				}
			}
		}

		//----------
		void Scroll::draw(DrawArguments& args) {
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
		}

		//----------
		void Scroll::mouse(MouseArguments& args) {
			this->elements->mouseAction(args);
			if (args.action == MouseArguments::Pressed) {
				this->onScrollBar = args.local.x > this->getWidth() - OFXCVGUI_SCROLL_AREA_WIDTH;
				this->dragTaken = args.isTaken();
			} else if (this->getMouseState() == Element::Dragging && this->length > this->getHeight() && !dragTaken) {
				if (this->onScrollBar) {
					const float range = this->length - this->getHeight();
					const float spareScrollSpace = this->getHeight() - this->getBarLength();
					this->setScroll(this->position + args.movement.y * range / spareScrollSpace);
				} else {
					this->setScroll(this->position - args.movement.y);
				}
			}
		}

		//----------
		void Scroll::keyboard(KeyboardArguments& args) {
			this->elements->keyboardAction(args);
		}

		//----------
		void Scroll::arrange(const BoundsChangeArguments & args) {
			float y = 0;
			for(auto element : this->elements->getElements()) {
				auto elementBounds = element->getBounds();
				elementBounds.y = y;
				elementBounds.width = this->getWidth() - OFXCVGUI_SCROLL_AREA_WIDTH;
				element->setBounds(elementBounds);
				y += elementBounds.height + OFXCVGUI_SCROLL_SPACING;
			}
			this->length = y;
			this->elements->setBounds(ofRectangle(0, 0, this->getWidth(), this->length));
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