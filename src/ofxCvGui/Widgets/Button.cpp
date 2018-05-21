#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		Button::Button(string caption, char hotKey) : Toggle() {
			this->init(caption, hotKey);
		}

		//----------
		Button::Button(string caption, std::function<void ()> action, char hotKey) {
			this->init(caption, hotKey);
			this->onHit += action;
		}

		//----------
		void Button::init(string caption, char hotKey) {
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