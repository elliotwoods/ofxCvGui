#include "ofxCvGui.h"

namespace ofxCvGui {
	void inspect(ofxCvGui::Widgets::IInspectable & target) {
		ofxCvGui::Panels::Inspector::setSelection(target);
	}
}