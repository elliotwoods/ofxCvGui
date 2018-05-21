#include "pch_ofxCvGui.h"


namespace ofxCvGui {
	namespace Panels {
		//----------
		Text::Text(string text) {
			this->text = text;

			this->onDraw.addListener([this] (DrawArguments & args) {
				this->drawText(args);
			}, this);
		}

		//----------
		Text::~Text() {
			this->onDraw.removeListeners(this);
		}

		//----------
		void Text::drawText(DrawArguments& arguments) {
			Utils::drawText(this->text, 10, 70, false);
		}
	}
}