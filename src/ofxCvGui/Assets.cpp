#include "Assets.h"

namespace ofxCvGui {
	//----------
	Assets::Assets() {
	}

	//----------
	void Assets::init() {
		string filename, name;
		
		string dataPath = "ofxCvGui";
		if (!ofDirectory::doesDirectoryExist(dataPath))
			dataPath = "../../../../../addons/ofxCvGui2/data/ofxCvGui";
		if (!ofDirectory::doesDirectoryExist(dataPath)) {
			ofLogError("ofxCvGui") << "Assets data path cannot be found. Be sure to copy the ofxCvGui folder from ofxCvGui2/data to your data folder!";
			return;
		}

		////
		//images
		////
		//
		string imageFolder = ofToDataPath(dataPath + "/images/", true);
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

		blankImage.allocate(30, 30, OF_IMAGE_GRAYSCALE);
		memset(blankImage.getPixels(), 0x46, 30*30);
		blankImage.update();
		//
		////


		////
		//fonts
		////
		//
		string fontFolder = ofToDataPath(dataPath + "/fonts", true);
		ofDirectory fonts;
		fonts.listDir(fontFolder);
		for (int i=0; i<fonts.size(); i++) {
			filename = fonts.getPath(i);
			if (ofFilePath::getFileExt(filename) != "ttf")
				continue;

			name = ofFilePath::getBaseName(filename);
			this->fonts.insert(pair<string, ofTrueTypeFont>(name + ":14", ofTrueTypeFont()));
			this->fonts[name + ":14"].loadFont(filename, 14);
			this->fonts.insert(pair<string, ofTrueTypeFont>(name + ":10", ofTrueTypeFont()));
			this->fonts[name + ":10"].loadFont(filename, 10);

			//set default font
			this->fonts["!"] = this->fonts[name + ":14"];
			ofLogNotice("ofxCvGui") << "Loaded font asset '" << name << "'" << endl;
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
			ofLogError("ofxCvGui") << "Image asset ['" << imageName << "'] will be instantiated with blank image";
			this->images[imageName] = this->blankImage;
			return this->blankImage;
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
	ofRectangle Assets::drawText(const string& text, float x, float y, const string &fontName, bool background, float minHeight, float minWidth) {
		ofPushStyle();
		ofSetColor(0x46);
		ofFill();
		bool useDefault = (fontName == "" || this->fonts.count(fontName) == 0);
		ofRectangle bounds(x, y, 0, 0);
		if (Assets::fonts.size() > 0) {
			ofTrueTypeFont& font(useDefault ? fonts.begin()->second : this->getFont(fontName));
			float rawWidth = font.getStringBoundingBox(text, x, y).width;
			float rawHeight = font.getStringBoundingBox("Hy", x, y).height;
			bounds.x = x;
			bounds.y = y;
			bounds.width = rawWidth + font.getSize();
			if (bounds.width < minWidth) {
				x+= (minWidth - bounds.width) / 2.0f;
				bounds.width = minWidth;
			}
			bounds.height = MAX(rawHeight, minHeight);
			if (background)
				ofRect(bounds);
			ofPopStyle();
			ofSetColor(255);
			font.drawString(text, x + font.getSize() / 2, y + (bounds.height + rawHeight * 2.0f / 3.0f) / 2.0f);
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
	//-----------
	Assets AssetRegister = Assets();
}