#include "Button.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		Button::Button(string caption) : Toggle() {
			this->init(caption);
		}

		//----------
		Button::Button(string caption, std::function<void ()> action) {
			this->init(caption);
			this->onHit += [action] (EventArgs &) {
				action();
			};
		}

		//----------
		void Button::init(string caption) {
			this->hitValue.set(caption, false);
			Toggle::setParameter(this->hitValue);
			this->onValueChange += [this] (ofParameter<bool> & value) {
				if (value) {
					this->onHit(EventArgs());
					this->hitValue.set(false);
				}
			};
		}
	}
}