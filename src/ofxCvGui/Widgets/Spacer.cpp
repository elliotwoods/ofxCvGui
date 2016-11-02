#include "Spacer.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		Spacer::Spacer() {
			this->setBounds(ofRectangle(0, 0, 100, 25));
			this->onDraw += [] (DrawArguments & args) {
				auto center = args.localBounds.getCenter();

				const auto spread = ofVec2f(-10.0f, 2.0f);
				ofDrawRectRounded(ofRectangle(center - spread, center + spread), 2.0f);
			};
		}
	}
}
