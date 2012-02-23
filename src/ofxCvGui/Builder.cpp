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
}