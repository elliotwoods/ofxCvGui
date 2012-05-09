#include "ofxCvGui/Panels/BaseImage.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		void BaseImage::setAutoRefresh(bool autoRefresh) {
			this->autoRefresh = autoRefresh;
		}

		//----------
		BaseImage::BaseImage() {
			this->autoRefresh = true;
		}

		//----------
		void BaseImage::drawPanel(const DrawArguments& arguments) {
			this->drawImage(arguments);
		}

		//----------
		void BaseImage::drawToolbar(float x) {
			AssetRegister["zoom_in"].draw(x, 20);
			AssetRegister["zoom_out"].draw(x + 40, 20);
		}
	}
}