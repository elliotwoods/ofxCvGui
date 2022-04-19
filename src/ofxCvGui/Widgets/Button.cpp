#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		Button::Button(string caption, int hotKey) : Toggle() {
			this->init(caption, hotKey);
		}

		//----------
		Button::Button(string caption, std::function<void ()> action, int hotKey) {
			this->init(caption, hotKey);
			this->onHit += action;
		}

		//----------
		void Button::init(string caption, int hotKey) {
			this->hitValue.set(caption, false);
			Toggle::setParameter(this->hitValue);

			if (hotKey != 0) {
				Toggle::setHotKey(hotKey);
			}

			this->onValueChange += [this](ofParameter<bool> & value) {
				if (value) {
					//button was pressed
					this->needsToDrop = true;
					this->onHit.notifyListeners();
				}
			};

			this->onUpdate += [this](UpdateArguments & args) {
				if (this->needsToDrop) {
					this->hitValue.set(false);
					this->needsToDrop = false;
				}
			};

			this->needsToDrop = false;
		}
	}
}