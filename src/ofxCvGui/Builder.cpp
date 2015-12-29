#include "ofxCvGui/Builder.h"

#define MAKE_PANEL_AND_RETURN auto newPanel = this->makePanel(asset, caption); PanelPtr panel(newPanel); controller.add(panel); return newPanel;
#define LABEL_PANEL_AND_RETURN newPanel->setCaption(caption); return newPanel;
namespace ofxCvGui {
	//----------
	void Builder::init() {
		PanelGroupPtr rootGroup( new Panels::Groups::Grid() );
		controller.init(rootGroup);
	}
	
#pragma mark add
	//----------
	void Builder::add(PanelPtr panel) {
		controller.add(panel);
	}

	//----------
	shared_ptr<Panels::Draws> Builder::add(ofBaseDraws& asset, string caption) {
		MAKE_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Image> Builder::add(ofImage& asset, string caption) {
		MAKE_PANEL_AND_RETURN
	}
	
    //----------
	shared_ptr<Panels::Pixels> Builder::add(const ofPixels& asset, string caption) {
		MAKE_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::PixelsVector> Builder::add(const vector<ofPixels>& asset, string caption) {
		MAKE_PANEL_AND_RETURN
	}
    
	//----------
	shared_ptr<Panels::Base> Builder::addBlank(string caption) {
		auto newPanel = this->makeBlank();
		newPanel->setCaption(caption);
		auto panel = PanelPtr(newPanel);
		this->controller.add(panel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::World> Builder::addWorld(string caption) {
		auto newPanel = this->makeWorld(caption);
		auto panel = PanelPtr(newPanel);
		this->controller.add(panel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::ElementHost> Builder::addElementHost(string caption) {
		auto newPanel = this->makeElementHost(caption);
		auto panel = PanelPtr(newPanel);
		this->controller.add(panel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Widgets> Builder::addWidgets(string caption) {
		auto newPanel = this->makeWidgets(caption);
		auto panel = PanelPtr(newPanel);
		this->controller.add(panel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Instructions> Builder::addInstructions() {
		auto newPanel = this->makeInstructions();
		auto panel = PanelPtr(newPanel);
		this->controller.add(panel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Inspector> Builder::addInspector() {
		auto newPanel = this->makeInspector();
		auto panel = PanelPtr(newPanel);
		this->controller.add(panel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Groups::Grid> Builder::addGrid() {
		auto newPanel = this->makeGrid();
		auto panel = PanelPtr(newPanel);
		this->controller.add(panel);
		return newPanel;
	}

#pragma mark make
	//----------
	shared_ptr<Panels::Draws> Builder::makePanel(ofBaseDraws& asset, string caption) {
		auto newPanel = shared_ptr<Panels::Draws> ( new Panels::Draws(asset) );
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Image> Builder::makePanel(ofImage& asset, string caption) {
		auto newPanel = shared_ptr<Panels::Image> ( new Panels::Image(asset) );
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Pixels> Builder::makePanel(const ofPixels& asset, string caption) {
		auto newPanel = shared_ptr<Panels::Pixels> ( new Panels::Pixels(asset) );
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::PixelsVector> Builder::makePanel(const vector<ofPixels>& asset, string caption) {
		auto newPanel = shared_ptr<Panels::PixelsVector> (new Panels::PixelsVector(asset) );
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Instructions> Builder::makeInstructions() {
		return shared_ptr<Panels::Instructions>( new Panels::Instructions() );
	}

	//----------
	shared_ptr<Panels::Inspector> Builder::makeInspector() {
		return shared_ptr<Panels::Inspector>( new Panels::Inspector() );
	}

	//----------
	shared_ptr<Panels::Groups::Grid> Builder::makeGrid() {
		return shared_ptr<Panels::Groups::Grid>( new Panels::Groups::Grid() );
	}

	//----------
	shared_ptr<Panels::World> Builder::makeWorld(string caption) {
		auto newPanel = shared_ptr<Panels::World>(new Panels::World());
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::ElementHost> Builder::makeElementHost(string caption) {
		auto newPanel = shared_ptr<Panels::ElementHost>(new Panels::ElementHost());
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Widgets> Builder::makeWidgets(string caption) {
		auto newPanel = shared_ptr<Panels::Widgets>(new Panels::Widgets());
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Base> Builder::makeBlank() {
		return shared_ptr<Panels::Base>(new Panels::Base());
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