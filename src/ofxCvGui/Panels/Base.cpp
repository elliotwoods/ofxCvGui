#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
		void Base::drawElement(const DrawArguments& arguments) {
			this->drawPanel(arguments);
			ofRectangle bounds = AssetRegister.drawText(this->caption, 20, 20, "", true, 30);
			this->drawToolbar(bounds.width + 40);
		}
	}
}