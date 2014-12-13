#include "ElementHost.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		ElementHost::ElementHost() {
			this->elementGroup = ElementGroupPtr(new ElementGroup());
			this->elementGroup->addListenersToParent(this, true);
		}

		//----------
		ElementGroupPtr ElementHost::getElementGroup() {
			return this->elementGroup;
		}
	}
}