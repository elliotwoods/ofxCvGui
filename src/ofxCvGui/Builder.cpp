#include "ofxCvGui/Builder.h"

namespace ofxCvGui {
	//----------
	void Builder::init() {
		PanelGroupPtr rootGroup( new Panels::Groups::Grid() );
		controller.init(rootGroup);
	}
	
	//----------
	ofPtr<Panels::Draws> Builder::add(ofBaseDraws& asset, string caption) {
		ofPtr<Panels::Draws> newPanel( new Panels::Draws(asset) );
		newPanel->setCaption(caption);
		controller.add( PanelPtr(newPanel) );
		return newPanel;
	}

	//----------
	ofPtr<Panels::Pixels> Builder::add(const ofPixels& asset, string caption) {
		ofPtr<Panels::Pixels> newPanel( new Panels::Pixels(asset) );
		newPanel->setCaption(caption);
		controller.add( PanelPtr(newPanel) );
		return newPanel;
	}

	//----------
	ofPtr<Panels::PixelsVector> Builder::add(const vector<ofPixels>& asset, string caption) {
		ofPtr<Panels::PixelsVector> newPanel;
		newPanel->setCaption(caption);
		controller.add( PanelPtr(newPanel) );
		return newPanel;
	}

	//----------
	ofPtr<Panels::Node> Builder::add(ofNode & asset, string caption) {
		ofPtr<Panels::Node> newPanel( new Panels::Node(asset) );
		newPanel->setCaption(caption);
		controller.add( PanelPtr(newPanel) );
		return newPanel;
	}

	//----------
	ofPtr<Panels::Instructions> Builder::addInstructions() {
		ofPtr<Panels::Instructions> newPanel( new Panels::Instructions() );
		controller.add( PanelPtr(newPanel) );
		return newPanel;
	}

	//----------
	ofPtr<Panels::Groups::Grid> Builder::addGrid() {
		ofPtr<Panels::Groups::Grid> newPanel( new Panels::Groups::Grid() );
		controller.add( PanelPtr(newPanel) );
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