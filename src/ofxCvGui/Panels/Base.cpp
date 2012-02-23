#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
		void Base::drawElement(const DrawArguments& arguments) {
			this->drawPanel(arguments);

			ofPushStyle();
			ofSetLineWidth(1);
			ofNoFill();
			ofRect(localBounds);
			ofPopStyle();
		}
	}
}