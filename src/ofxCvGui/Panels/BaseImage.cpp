#include "ofxCvGui/Panels/BaseImage.h"

namespace ofxCvGui {
	extern Assets AssetRegister;
	namespace Panels {

		void BaseImage::drawPanel(const DrawArguments& arguments) {
			this->drawImage(arguments);

			AssetRegister["zoom_in"].draw(20, 20);
			AssetRegister["zoom_out"].draw(60, 20);
			AssetRegister.drawText(this->caption, 100, 20, "", true, 30);
		}
	}
}