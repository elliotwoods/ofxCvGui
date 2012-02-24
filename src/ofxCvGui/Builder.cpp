#include "ofxCvGui/Builder.h"

namespace ofxCvGui {
	//----------
	void Builder::init() {
		PanelGroupPtr rootGroup(new Panels::Groups::Grid());
		controller.init(rootGroup);
	}
	
	//----------
	void Builder::add(ofBaseDraws& asset, string caption) {
		PanelPtr newPanel(new Panels::Draws(asset));
		newPanel->setCaption(caption);
		controller.add(newPanel);
	}

	//----------
	void Builder::add(const ofPixels& asset, string caption) {
		PanelPtr newPanel(new Panels::Pixels(asset));
		newPanel->setCaption(caption);
		controller.add(newPanel);
	}

	//----------
	void Builder::add(const vector<ofPixels>& asset, string caption) {
		PanelPtr newPanel(new Panels::PixelsVector(asset));
		newPanel->setCaption(caption);
		controller.add(newPanel);
	}
}