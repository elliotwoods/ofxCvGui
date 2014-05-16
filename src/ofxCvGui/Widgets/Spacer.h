#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Spacer : public Element {
		public:
			Spacer();
		};

		shared_ptr<Spacer> makeSpacer();
	}
}