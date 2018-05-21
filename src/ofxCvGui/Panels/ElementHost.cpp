#include "pch_ofxCvGui.h"

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

		//----------
		shared_ptr<Panels::ElementHost> makeElementHost(string caption) {
			auto newPanel = shared_ptr<Panels::ElementHost>(new Panels::ElementHost());
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}