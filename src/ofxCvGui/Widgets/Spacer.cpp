#include "Spacer.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		Spacer::Spacer() {
			this->setBounds(ofRectangle(0, 0, 100, 30));
			this->onDraw += [] (DrawArguments & args) {
				auto center = args.localBounds.getCenter();

				ofCircle(center + ofVec2f(-10.0f, 0.0f), 2.0f);
				ofCircle(center, 2.0f);
				ofCircle(center + ofVec2f(+10.0f, 0.0f), 2.0f);
			};
		}
	}
}
