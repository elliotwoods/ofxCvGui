#include "ofxCvGui.h"

namespace ofxCvGui {
	void inspect(ofxCvGui::Widgets::IInspectable & target) {
		Panels::Inspector::setSelection(target);
	}

	void addToInspector(ofxCvGui::ElementPtr element) {
		Panels::Inspector::addWidget(element);
	}
}