#include "ofxCvGui/Builder.h"

namespace ofxCvGui {
	//----------
	void Builder::init() {
		PanelGroupPtr rootGroup(new Panels::Groups::Grid());
		controller.init(rootGroup);
	}
	
	//----------
	PanelPtr Builder::add(ofBaseDraws& asset, string caption) {
		PanelPtr newPanel(new Panels::Draws(asset));
		newPanel->setCaption(caption);
		controller.add(newPanel);
		return newPanel;
	}

	//----------
	PanelPtr Builder::add(const ofPixels& asset, string caption) {
		PanelPtr newPanel(new Panels::Pixels(asset));
		newPanel->setCaption(caption);
		controller.add(newPanel);
		return newPanel;
	}

	//----------
	PanelPtr Builder::add(const vector<ofPixels>& asset, string caption) {
		PanelPtr newPanel(new Panels::PixelsVector(asset));
		newPanel->setCaption(caption);
		controller.add(newPanel);
		return newPanel;
	}

	//----------
	PanelPtr Builder::addInstructions() {
		PanelPtr newPanel(new Panels::Instructions());
		controller.add(newPanel);
		return newPanel;
	}

	//----------
	void Builder::drop(PanelPtr &panel) {
		controller.drop(panel);
	}

	//----------
	void Builder::clear() {
		controller.clear();
	}
}