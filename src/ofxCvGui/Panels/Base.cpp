#include "ofxCvGui/Panels/Base.h"
#include "../../../ofxAssets/src/ofxAssets.h"

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
			Utils::drawText(this->caption, 20, 20, true, 30);
		}
	}
}