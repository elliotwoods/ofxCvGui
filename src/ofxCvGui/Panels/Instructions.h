#pragma once
#include "ofxCvGui/Panels/Text.h"
#include <fstream>

namespace ofxCvGui {
	namespace Panels {
		class Instructions : public Text {
		public:
			Instructions();
		};

		shared_ptr<Panels::Instructions> makeInstructions();
	}
}