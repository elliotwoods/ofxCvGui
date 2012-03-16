#pragma once
#include "ofImage.h"
#include "ofGraphics.h"
#include "ofTrueTypeFont.h"
#include <map>

using namespace std;
namespace ofxCvGui {
	class Assets {
	public:
		Assets();
		void init();
		ofImage& operator[](const string& imageName);
		ofImage& getImage(const string& imageName);
		ofTrueTypeFont& getFont(const string& fontName);

		ofRectangle drawText(const string& text, float x, float y, const string& fontName="", bool background=true, float minHeight=15, float minWidth=0);
	protected:
		map<string, ofImage> images;
		map<string, ofTrueTypeFont> fonts;

		ofImage blankImage;
		ofTrueTypeFont blankFont;
	};

	extern Assets AssetRegister;
}