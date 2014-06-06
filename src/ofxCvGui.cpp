#include "ofxCvGui.h"

namespace ofxCvGui {
	//-----------
	void inspect(ofxCvGui::Widgets::IInspectable & target) {
		Panels::Inspector::setSelection(target);
	}

	//-----------
	bool isBeingInspected(ofxCvGui::Widgets::IInspectable & target) {
		auto inspectedSelection = Panels::Inspector::getSelection();
		return inspectedSelection.find(& target) != inspectedSelection.end();
	}

	//-----------
	void addToInspector(ofxCvGui::ElementPtr element) {
		Panels::Inspector::addWidget(element);
	}
}