#include "ofxCvGui/Builder.h"

namespace ofxCvGui {
	//----------
	void Builder::init() {
		PanelGroupPtr rootGroup( new Panels::Groups::Grid() );
		controller.init(rootGroup);
	}
	
	//----------
	void Builder::add(PanelPtr panel) {
		controller.add(panel);
	}

	//----------
	shared_ptr<Panels::Draws> Builder::add(ofBaseDraws& asset, string caption) {
		auto newPanel = Panels::makeBaseDraws(asset, caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Image> Builder::add(ofImage& asset, string caption) {
		auto newPanel = Panels::makeImage(asset, caption);
		this->controller.add(newPanel);
		return newPanel;
	}
	
    //----------
	shared_ptr<Panels::Pixels> Builder::add(const ofPixels& asset, string caption) {
		auto newPanel = Panels::makePixels(asset, caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::PixelsVector> Builder::add(const vector<ofPixels>& asset, string caption) {
		auto newPanel = Panels::makePixelsVector(asset, caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Texture> Builder::add(const ofTexture & asset, string caption) {
		auto newPanel = Panels::makeTexture(asset, caption);
		this->controller.add(newPanel);
		return newPanel;
	}
    
	//----------
	shared_ptr<Panels::Base> Builder::addBlank(string caption) {
		auto newPanel = Panels::makeBlank(caption);
		newPanel->setCaption(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::World> Builder::addWorld(string caption) {
		auto newPanel = Panels::makeWorld(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::ElementHost> Builder::addElementHost(string caption) {
		auto newPanel = Panels::makeElementHost(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::ElementCanvas> Builder::addElementCanvas(string caption) {
		auto newPanel = Panels::makeElementCanvas(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Widgets> Builder::addWidgets(string caption) {
		auto newPanel = Panels::makeWidgets(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Instructions> Builder::addInstructions() {
		auto newPanel = Panels::makeInstructions();
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Inspector> Builder::addInspector() {
		auto newPanel = Panels::makeInspector();
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Tree> Builder::addTree() {
		auto newPanel = Panels::makeTree();
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Groups::Grid> Builder::addGrid() {
		auto newPanel = Panels::Groups::makeGrid();
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Groups::Strip> Builder::addStrip() {
		auto newPanel = Panels::Groups::makeStrip();
		this->controller.add(newPanel);
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