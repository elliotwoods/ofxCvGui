#include "ofxCvGui/Panels/Text.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Text::Text(string text) {
			this->text = text;
		}

		//----------
		void Text::drawPanel(const DrawArguments& arguments) {
			AssetRegister.drawText(this->text, 10, 70, "", false);
		}
	}
}