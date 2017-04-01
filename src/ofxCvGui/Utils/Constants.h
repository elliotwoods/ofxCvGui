#pragma once
#include <string>

#ifdef OFXCVGUI_EXPORT_ENABLED
	#define OFXCVGUI_API_ENTRY __declspec(dllexport)
#else
	#define OFXCVGUI_API_ENTRY
#endif

namespace ofxCvGui {
	std::string getDefaultTypeface();
}