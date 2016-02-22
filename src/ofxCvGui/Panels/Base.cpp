#include "ofxCvGui/Panels/Base.h"
#include "ofxAssets.h"

using namespace ofxAssets;

namespace ofxCvGui {
	namespace Panels {
		//-----------
		Base::Base() {
			this->setScissor(true);
			this->onDraw.addListener([this] (ofxCvGui::DrawArguments & args) {
				if (args.chromeEnabled) {
					this->drawTitle();
				}
			}, 1000, this);
		}
		
		//----------
		void Base::drawTitle() {
			if (!this->caption.empty()) {
				Utils::drawText(this->caption, 20, 20, true, 30);
			}
		}

		//----------
		PanelPtr makeBlank(string caption) {
			auto newPanel = make_shared<Base>();
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}