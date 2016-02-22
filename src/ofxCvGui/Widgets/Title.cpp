#include "Title.h"
#include "../Utils/Constants.h"
#include "ofxAssets.h"
namespace ofxCvGui {
	namespace Widgets {
		//----------
		Title::Title(string caption, Level level) :
		level(level) {
			this->setCaption(caption);
			this->setBounds(ofRectangle(5, 0, 100, 40));
			
			//fix font size
			int fontSize = 14;
			switch (this->level) {
			case H1:
				fontSize = 24;
				break;
			case H2:
				fontSize = 18;
				break;
			case H3:
				fontSize = 14;
				break;
			}

			//check if we need to increase height
			auto & font = ofxAssets::font(ofxCvGui::getDefaultTypeface(), fontSize);
			auto textBounds = font.getStringBoundingBox(this->caption, 0, 30);
			const auto textBottom = textBounds.getBottom();
			if (textBottom + 10 > this->getHeight()) {
				this->setHeight(textBottom + 10);
			}
			this->onDraw += [this, fontSize] (ofxCvGui::DrawArguments & args) {
				auto & font = ofxAssets::font(ofxCvGui::getDefaultTypeface(), fontSize);
				font.drawString(this->caption, 0, 30);
			};
		}
	}
}