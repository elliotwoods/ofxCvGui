#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Instructions::Instructions() {
			this->caption = "Instructions";
			ifstream file;
			string filePath = ofToDataPath("instructions.txt", true);
			file.open(filePath.c_str());
			this->text = string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		}

		//----------
		shared_ptr<Panels::Instructions> makeInstructions() {
			return shared_ptr<Panels::Instructions>(new Panels::Instructions());
		}
	}
}