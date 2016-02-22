#include "Widgets.h"

using namespace ofxCvGui::Widgets;

namespace ofxCvGui {
	namespace Panels {
		//----------
		shared_ptr<Panels::Widgets> makeWidgets(string caption) {
			auto newPanel = shared_ptr<Panels::Widgets>(new Panels::Widgets());
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}