#pragma once
#include <string>

#define OFXCVGUI_MAKE(T, ...) shared_ptr<T>(new T(__VA_ARGS__))
namespace ofxCvGui {
	extern std::string defaultTypeface;
}