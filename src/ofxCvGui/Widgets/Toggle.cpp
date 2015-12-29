#include "Toggle.h"
#include "../../../addons/ofxAssets/src/ofxAssets.h"

using namespace ofxAssets;

namespace ofxCvGui {
	namespace Widgets {
		//----------
		Toggle::Toggle(ofParameter<bool> & parameter, char hotKey) {
			this->setParameter(parameter);
			this->setCaption(this->value->getName());
			this->hotKey = hotKey;
			this->init();
		}

		//----------
		Toggle::Toggle(string caption, char hotKey) {
			this->setParameter(*new ofParameter<bool>(caption, false));
			this->localAllocation = true;
			this->hotKey = hotKey;
			this->init();
		}

		//----------
		Toggle::Toggle(string caption, function<bool()> get, function<void(bool)> set, char hotKey) :
		Toggle(caption, hotKey) {
			this->onUpdate += [this, get](UpdateArguments &) {
				this->value->set(get());
			};
			this->onValueChange += [this, set](ofParameter<bool> & value) {
				set(value.get());
			};
		}

		//----------
		Toggle::Toggle() {
			this->value = nullptr;
			this->init();
		}

		//----------
		Toggle::~Toggle() {
			if(this->localAllocation) {
				delete this->value;
			}
		}

		//----------
		void Toggle::init() {
			this->setBounds(ofRectangle(5, 0, 100, 40));

			this->onUpdate += [this] (UpdateArguments & args) {
				this->update(args);
			};

			this->onDraw += [this] (DrawArguments & args) {
				this->draw(args);
			};

			this->onMouse += [this] (MouseArguments & args) {
				this->mouseAction(args);
			};
			this->onMouseReleased += [this] (MouseArguments & args) {
				this->mouseReleased(args);
			};
			this->onBoundsChange += [this] (BoundsChangeArguments & args) {
				this->boundsChange(args);
			};

			this->onKeyboard += [this](ofxCvGui::KeyboardArguments & keyArgs) {
				if (keyArgs.action == ofxCvGui::KeyboardArguments::Action::Pressed) {
					if (keyArgs.key == this->hotKey) {
						this->toggle();
					}
				}
			};
			
			this->isMouseOver = false;
		}

		//----------
		void Toggle::setParameter(ofParameter<bool> & parameter) {
			this->value = & parameter;
			this->setCaption(this->value->getName());
		}

		//----------
		ofParameter<bool> & Toggle::getParameter() {
			return * this->value;
		}

		//----------
		void Toggle::setHotKey(char hotKey) {
			this->hotKey = hotKey;
		}

		//----------
		char Toggle::getHotKey() const {
			return this->hotKey;
		}

		//----------
		void Toggle::update(UpdateArguments &) {

		}

		//----------
		void Toggle::draw(DrawArguments & args) {
			if (!this->value) {
				//nothing allocated
				return;
			}

			auto & font = ofxAssets::font(ofxCvGui::defaultTypeface, 12);
			auto isMouseDown = this->getMouseState() != LocalMouseState::Waiting;

			ofPushStyle();
			
			//fill
			ofSetColor(this->value->get() ^ isMouseDown ?  80 : 50);
			ofFill();
			const auto radius = 4.0f;
			ofDrawRectRounded(this->buttonBounds, radius, radius, radius, radius);
			
			//outline
			if (this->isMouseOver) {
				ofNoFill();
				ofSetColor(!this->value->get() ?  80 : 50);
				ofDrawRectRounded(this->buttonBounds, radius, radius, radius, radius);
			}

			ofSetColor(255);
			auto caption = this->caption;
			if (this->hotKey != 0) {
				caption = caption + " [" + Utils::makeString(this->hotKey) + "]";
			}
			Utils::drawText(caption, this->buttonBounds, false);
			
			ofPopStyle();

			//draw side marker
			ofPushStyle();
			ofSetLineWidth(1.0f);
			ofDrawLine(this->getWidth(), 0, this->getWidth(), this->buttonBounds.getHeight());
			ofPopStyle();
		}

		//----------
		void Toggle::mouseAction(MouseArguments & args) {
			if (!this->value) {
				//nothing allocated
				return;
			}

			this->isMouseOver = this->buttonBounds.inside(args.local);

			switch(args.action) {
			case MouseArguments::Pressed:
				if (this->isMouseOver) {
					args.takeMousePress(this);
				}
				break;
			}
		}

		//----------
		void Toggle::mouseReleased(MouseArguments & args) {
			this->toggle();
		}

		//----------
		void Toggle::boundsChange(BoundsChangeArguments & args) {
			this->buttonBounds = args.localBounds;
			buttonBounds.height -= 10;
			buttonBounds.width -= 10;
		}

		//----------
		void Toggle::toggle() {
			this->value->set(!this->value->get());
			this->notifyValueChange();
		}

		//----------
		void Toggle::notifyValueChange() {
			this->onValueChange.notifyListeners(* this->value);
		}
	}
}