#pragma once
#include <string>

#ifdef OFXCVGUI_EXPORT_LIBRARY
	#define OFXCVGUI_EXPORTS __declspec(dllexport)
#else
	#define OFXCVGUI_EXPORTS
#endif

namespace ofxCvGui {
	std::string getDefaultTypeface();
}