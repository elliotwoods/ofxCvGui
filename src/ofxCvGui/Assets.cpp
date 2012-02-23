#include "Assets.h"

namespace ofxCvGui {
	void Assets::init() {
		string filename, name;
		
		////
		//images
		////
		//
		string imageFolder = ofToDataPath("ofxCvGui/images/", true);
		ofDirectory images;
		images.listDir(imageFolder);
		for (int i=0; i<images.size(); i++) {
			filename = images.getPath(i);
			if (ofFilePath::getFileExt(filename) != "png")
				continue;

			name = ofFilePath::getBaseName(filename);
			this->images.insert(pair<string, ofImage>(name, ofImage()));
			this->images[name].loadImage(filename);
			this->images[name].update();

			ofLogNotice("ofxCvGui") << "Loaded image asset '" << name << "'" << endl;
		}
		//
		////


		////
		//fonts
		////
		//
		string fontFolder = ofToDataPath("ofxCvGui/fonts", true);
		ofDirectory fonts;
		fonts.listDir(fontFolder);
		for (int i=0; i<fonts.size(); i++) {
			filename = fonts.getPath(i);
			if (ofFilePath::getFileExt(filename) != "ttf")
				continue;

			name = ofFilePath::getBaseName(filename);
			this->fonts.insert(pair<string, ofTrueTypeFont>(name, ofTrueTypeFont()));
			this->fonts[name].loadFont(filename, 14);

			ofLogNotice("ofxCvGui") << "Loaded image asset '" << name << "'" << endl;
		}
		//
		////
	}

	//-----------
	ofImage& Assets::operator[](const string& imageName) {
		return this->getImage(imageName);
	}

	//-----------
	ofImage& Assets::getImage(const string& imageName) {
		if (this->images.count(imageName) == 0) {
			ofLogError("ofxCvGui") << "Image asset ['" << imageName << "'] not found";
			return ofImage();
		}
		return this->images[imageName];
	}

	//-----------
	ofTrueTypeFont& Assets::getFont(const string& fontName) {
		if (this->fonts.count(fontName) == 0) {
			ofLogError("ofxCvGui") << "Font asset ['" << fontName << "'] not found";
			return ofTrueTypeFont();
		}
		return this->fonts[fontName];
	}

	//-----------
	ofRectangle Assets::drawText(const string& text, float x, float y, const string &fontName, bool background, float minHeight) {
		ofPushStyle();
		ofSetColor(0x46);
		ofFill();
		bool useDefault = (fontName == "" || this->fonts.count(fontName) == 0);
		ofRectangle bounds;
		if (this->fonts.size() > 0) {
			ofTrueTypeFont& font(useDefault ? fonts.begin()->second : this->getFont(fontName));
			bounds = font.getStringBoundingBox(text, x, y);
			float rawHeight = bounds.height;
			bounds.y += bounds.height;
			bounds.height = MAX(bounds.height, minHeight);
			bounds.width += font.getSize() * 2.0f;
			if (background)
				ofRect(bounds);
			ofPopStyle();
			ofSetColor(255);
			font.drawString(text, x + font.getSize(), y + (bounds.height + rawHeight) / 2.0f);
		} else {
			bounds = ofRectangle(x, y, text.length() * 10 + 20, 30);
			if (background)
				ofRect(bounds);
			ofPopStyle();
			ofSetColor(255);
			ofDrawBitmapString(text, x + 10, y + 20);
		}

		return bounds;
	}

	//-----------
	Assets AssetRegister;
}