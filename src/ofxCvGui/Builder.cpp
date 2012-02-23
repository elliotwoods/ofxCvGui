#include "ofxCvGui/Builder.h"

namespace ofxCvGui {
	//----------
	void Builder::init() {
		mainGrid = new Panels::Groups::Grid();
		rootPanel = mainGrid;
		controller.init(rootPanel);
	}

	//----------
	void Builder::add(ofBaseDraws& asset, string caption) {
		Panels::Base* newPanel = new Panels::BaseDraws(asset);
		newPanel->setCaption(caption);
		controller.add(newPanel);
	}
}