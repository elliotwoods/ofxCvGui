#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	//----------
	void Builder::init() {
		PanelGroupPtr rootGroup( new Panels::Groups::Grid() );
		Controller::X().init(rootGroup);
	}
	
	//----------
	void Builder::add(PanelPtr panel) {
		Controller::X().add(panel);
	}

	//----------
	shared_ptr<Panels::Draws> Builder::add(ofBaseDraws& asset, string caption) {
		auto newPanel = Panels::makeBaseDraws(asset, caption);
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Image> Builder::add(ofImage& asset, string caption) {
		auto newPanel = Panels::makeImage(asset, caption);
		Controller::X().add(newPanel);
		return newPanel;
	}
	
    //----------
	shared_ptr<Panels::Pixels> Builder::add(const ofPixels& asset, string caption) {
		auto newPanel = Panels::makePixels(asset, caption);
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::PixelsVector> Builder::add(const vector<ofPixels>& asset, string caption) {
		auto newPanel = Panels::makePixelsVector(asset, caption);
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Texture> Builder::add(const ofTexture & asset, string caption) {
		auto newPanel = Panels::makeTexture(asset, caption);
		Controller::X().add(newPanel);
		return newPanel;
	}
    
	//----------
	shared_ptr<Panels::Base> Builder::addBlank(string caption) {
		auto newPanel = Panels::makeBlank(caption);
		newPanel->setCaption(caption);
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::World> Builder::addWorld(string caption) {
		auto newPanel = Panels::makeWorld(caption);
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::ElementHost> Builder::addElementHost(string caption) {
		auto newPanel = Panels::makeElementHost(caption);
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::ElementCanvas> Builder::addElementCanvas(string caption) {
		auto newPanel = Panels::makeElementCanvas(caption);
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Widgets> Builder::addWidgets(string caption) {
		auto newPanel = Panels::makeWidgets(caption);
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Instructions> Builder::addInstructions() {
		auto newPanel = Panels::makeInstructions();
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Inspector> Builder::addInspector() {
		auto newPanel = Panels::makeInspector();
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Tree> Builder::addTree() {
		auto newPanel = Panels::makeTree();
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Groups::Grid> Builder::addGrid() {
		auto newPanel = Panels::Groups::makeGrid();
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Groups::Strip> Builder::addStrip() {
		auto newPanel = Panels::Groups::makeStrip();
		Controller::X().add(newPanel);
		return newPanel;
	}

	//----------
	void Builder::remove(PanelPtr &panel) {
		Controller::X().remove(panel);
	}

	//----------
	void Builder::clear() {
		Controller::X().clear();
	}
}