#include "Button.h"

namespace ofxCvGui {
	namespace Utils {
		//----------
		Button::Button() {
			this->down = false;

			this->onMouse.addListener([this] (MouseArguments& args) {
				if (args.isLocalPressed()) {
					this->down = true;
				}
				if (args.action == MouseArguments::Released) {
					if(this->down && args.isLocal()) {
						ofVec2f local = args.localNormalised;
						this->onHit(local);
					}
					this->down = false;
				}
			}, this);

			this->onDraw.addListener([this] (DrawArguments& args) {
				if (this->down) {
					this->onDrawDown(args);
				} else {
					this->onDrawUp(args);
				}
			}, this);
		}

		//----------
		Button::~Button() {
			this->onMouse.removeListeners(this);
			this->onDraw.removeListeners(this);
		}
	}
}
