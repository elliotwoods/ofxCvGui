#include "Scroll.h"

#include "ofxCvGui/Utils/Utils.h"

#define OFXCVGUI_SCROLL_SPACING 10.0f
#define OFXCVGUI_SCROLL_AREA_WIDTH 20.0f
#define OFXCVGUI_SCROLL_BAR_WIDTH 5.0f
#define OFXCVGUI_GUTTER_WIDTH 5.0f

namespace ofxCvGui {
	namespace Panels {
		//----------
		Scroll::Scroll() {
			this->elements = ElementGroupPtr(new ElementGroup());
			this->elements->setScissorEnabled(true);

			this->onUpdate += [this](UpdateArguments & args) { this->update();	};
			this->onDraw += [this] (DrawArguments & args) { this->draw(args); };
			this->onMouse += [this] (MouseArguments & args) { this->mouse(args); };
			this->onKeyboard += [this] (KeyboardArguments & args) { this->keyboard(args); };
			this->onBoundsChange += [this] (BoundsChangeArguments & args) { this->arrangeScroll(); };
			this->position = 0.0f;
			this->length = 0.0f;
			this->onScrollBar = false;

			this->elements->addListenersToParent(this);

			//draw side lines
			this->elements->onDraw += [this](DrawArguments & args) {
				for (auto element : this->elements->getElements()) {
					auto color = ofxCvGui::Utils::toColor(typeid(*element.get()).name());
					ofPushStyle();
					{
						ofSetColor(color);
						const auto x = args.localBounds.width;
						const auto & bounds = element->getBounds();
						ofDrawLine(x, bounds.getTop(), x, bounds.getBottom());
					}
					ofPopStyle();
				}
			};
		}

		//----------
		void Scroll::add(ElementPtr element) {
			this->elements->add(element);
			this->arrangeScroll();
		}

		//----------
		void Scroll::clear() {
			this->elements->clear();
			this->arrangeScroll();
		}

		//----------
		ElementGroupPtr Scroll::getElementGroup() {
			return this->elements;
		}

		//----------
		void Scroll::setScroll(float position) {
			this->position = position;
			position = floor(position);
			if (this->elements->getBounds().y != -position) {
				this->elements->setPosition(ofVec2f(0, -position));
			}
		}

		//----------
		float Scroll::getScroll() const {
			return this->position;
		}

		//----------
		void Scroll::scrollToInclude(ElementPtr element) {
			const auto elementBounds = element->getBounds();
			auto correctedElementBounds = elementBounds;
			correctedElementBounds.y -= this->position;

			if (this->getLocalBounds().getIntersection(correctedElementBounds).getHeight() == 0) {
				//check if need to scroll up or down
				if (this->position > correctedElementBounds.y) {
					//need to scroll up
					this->setScroll(elementBounds.y);
				} else {
					//need to scroll down
					this->setScroll(elementBounds.height + (elementBounds.y - this->getLocalBounds().height));
				}
			}
		}

		//----------
		void Scroll::update() {
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
			float barLength = this->getBarLength();
			if (barLength < this->getHeight() - 2 * OFXCVGUI_SCROLL_AREA_WIDTH) {
				//draw bar only if we have excess contents which are scrollable
				float barPosition = this->getBarY();

				ofPushStyle();
				ofSetColor(255);
				float x = this->getWidth() - OFXCVGUI_SCROLL_AREA_WIDTH / 2.0f;
				ofSetLineWidth(0.0f);
				ofDrawCircle(x, barPosition, OFXCVGUI_SCROLL_BAR_WIDTH / 2.0f);
				ofDrawCircle(x, barPosition + barLength, OFXCVGUI_SCROLL_BAR_WIDTH / 2.0f);
				ofDrawRectangle(x - OFXCVGUI_SCROLL_BAR_WIDTH / 2.0f, barPosition, OFXCVGUI_SCROLL_BAR_WIDTH, barLength);
				ofPopStyle();
			}
		}

		//----------
		void Scroll::mouse(MouseArguments& args) {
			if (args.takeMousePress(this)) {
				this->onScrollBar = args.local.x > this->getWidth() - OFXCVGUI_SCROLL_AREA_WIDTH;
			} else if (args.isDragging(this)) {
				if (this->length > this->getHeight()) {
					if (this->onScrollBar) {
						const float range = this->length - this->getHeight();
						const float spareScrollSpace = this->getHeight() - this->getBarLength();
						this->setScroll(this->position + args.movement.y * range / spareScrollSpace);
					}
					else {
						this->setScroll(this->position - args.movement.y);
					}
				}
			}
		}

		//----------
		void Scroll::keyboard(KeyboardArguments& args) {

		}

		//----------
		void Scroll::arrangeScroll() {
			float y = 0;
			for(auto element : this->elements->getElements()) {
				auto elementBounds = element->getBounds();
				elementBounds.x = OFXCVGUI_GUTTER_WIDTH;
				elementBounds.y = y;
				elementBounds.width = this->getWidth() - (elementBounds.x + OFXCVGUI_SCROLL_AREA_WIDTH + OFXCVGUI_GUTTER_WIDTH * 2.0f);
				if (elementBounds.width <= 0 || elementBounds.height <= 0) {
					continue; // sometimes during initialisation this might happen, and it can cause errors
				}
				element->setBounds(elementBounds);
				y += elementBounds.height + OFXCVGUI_SCROLL_SPACING;
			}
			this->length = y;
			this->elements->setBounds(ofRectangle(0, -floor(position), this->getWidth() - OFXCVGUI_SCROLL_AREA_WIDTH, this->length));
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