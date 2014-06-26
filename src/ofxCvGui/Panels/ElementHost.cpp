#include "ElementHost.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		ElementHost::ElementHost() {
			this->elementGroup = ElementGroupPtr(new ElementGroup());
			this->onUpdate += [this] (UpdateArguments&) {
				this->elementGroup->update();
			};
			this->onDraw += [this] (DrawArguments& args) {
				ofPushView();
				ofViewport(args.globalBounds);
				ofSetupScreen();
				this->elementGroup->draw(args);
				ofPopView();
			};
			this->onMouse += [this] (MouseArguments& args) {
				this->elementGroup->mouseAction(args);
			};
			this->onKeyboard += [this] (KeyboardArguments& args) {
				this->elementGroup->keyboardAction(args);
			};
			this->onBoundsChange += [this] (BoundsChangeArguments& args) {
				localBounds = args.bounds;
				localBounds.x = 0.0f;
				localBounds.y = 0.0f;
				this->elementGroup->setBounds(localBounds);
			};
		}

		//----------
		ElementGroupPtr ElementHost::getElementGroup() {
			return this->elementGroup;
		}
	}
}