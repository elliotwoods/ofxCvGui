#include "ofxCvGui/Panels/Instructions.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Instructions::Instructions() {
			this->caption = "Instructions";
			ifstream file;
			file.open(ofToDataPath("instructions.txt"));
			this->text = string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		}
	}
}