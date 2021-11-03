#pragma once
#include <string>
#include "ofConstants.h"

#ifdef TARGET_WIN32
#	if defined(OFXCVGUI_EXPORTS_ENABLE)
#		define OFXCVGUI_API_ENTRY __declspec(dllexport)
#	elif defined(OFXCVGUI_IMPORTS_ENABLE)
#		define OFXCVGUI_API_ENTRY __declspec(dllimport)
#	else
#		define OFXCVGUI_API_ENTRY
#	endif
#else
#	define OFXCVGUI_API_ENTRY
#endif

namespace ofxCvGui {
	std::string OFXCVGUI_API_ENTRY getDefaultTypeface();
	std::string OFXCVGUI_API_ENTRY getGlyphTypeface();
	
}