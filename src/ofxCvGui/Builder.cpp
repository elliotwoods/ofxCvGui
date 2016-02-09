#include "ofxCvGui/Builder.h"

#define MAKE_PANEL_AND_RETURN auto newPanel = makePanel(asset, caption); PanelPtr panel(newPanel); controller.add(panel); return newPanel;
#define LABEL_PANEL_AND_RETURN newPanel->setCaption(caption); return newPanel;
namespace ofxCvGui {
#pragma mark make
	//----------
	shared_ptr<Panels::Draws> makePanel(ofBaseDraws& asset, string caption) {
		auto newPanel = make_shared<Panels::Draws>(asset);
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Image> makePanel(ofImage& asset, string caption) {
		auto newPanel = make_shared<Panels::Image>(asset);
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Pixels> makePanel(const ofPixels& asset, string caption) {
		auto newPanel = make_shared<Panels::Pixels>(asset);
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::PixelsVector> makePanel(const vector<ofPixels>& asset, string caption) {
		auto newPanel = make_shared<Panels::PixelsVector>(asset);
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Texture> makePanel(const ofTexture & asset, string caption) {
		auto newPanel = make_shared<Panels::Texture>(asset);
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::World> makeWorld(string caption) {
		auto newPanel = shared_ptr<Panels::World>(new Panels::World());
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::ElementHost> makeElementHost(string caption) {
		auto newPanel = shared_ptr<Panels::ElementHost>(new Panels::ElementHost());
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::ElementCanvas> makeElementCanvas(string caption) {
		auto newPanel = shared_ptr<Panels::ElementCanvas>(new Panels::ElementCanvas());
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Widgets> makeWidgets(string caption) {
		auto newPanel = shared_ptr<Panels::Widgets>(new Panels::Widgets());
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Tree> makeTree(string caption) {
		auto newPanel = make_shared<Panels::Tree>();
		LABEL_PANEL_AND_RETURN
	}

	//----------
	shared_ptr<Panels::Instructions> makeInstructions() {
		return shared_ptr<Panels::Instructions>(new Panels::Instructions());
	}

	//----------
	shared_ptr<Panels::Inspector> makeInspector() {
		return shared_ptr<Panels::Inspector>(new Panels::Inspector());
	}

	//----------
	shared_ptr<Panels::Groups::Grid> makeGrid() {
		return shared_ptr<Panels::Groups::Grid>(new Panels::Groups::Grid());
	}

	//----------
	shared_ptr<Panels::Groups::Strip> makeStrip(Panels::Groups::Strip::Direction direction) {
		return make_shared<Panels::Groups::Strip>(direction);
	}

	//----------
	shared_ptr<Panels::Base> makeBlank(string caption) {
		auto newPanel = shared_ptr<Panels::Base>(new Panels::Base());
		LABEL_PANEL_AND_RETURN
	}

#pragma mark Builder
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
	shared_ptr<Panels::Texture> Builder::add(const ofTexture & asset, string caption) {
		MAKE_PANEL_AND_RETURN
	}
    
	//----------
	shared_ptr<Panels::Base> Builder::addBlank(string caption) {
		auto newPanel = makeBlank(caption);
		newPanel->setCaption(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::World> Builder::addWorld(string caption) {
		auto newPanel = makeWorld(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::ElementHost> Builder::addElementHost(string caption) {
		auto newPanel = makeElementHost(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::ElementCanvas> Builder::addElementCanvas(string caption) {
		auto newPanel = makeElementCanvas(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Widgets> Builder::addWidgets(string caption) {
		auto newPanel = makeWidgets(caption);
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Instructions> Builder::addInstructions() {
		auto newPanel = makeInstructions();
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Inspector> Builder::addInspector() {
		auto newPanel = makeInspector();
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Tree> Builder::addTree() {
		auto newPanel = makeTree();
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Groups::Grid> Builder::addGrid() {
		auto newPanel = makeGrid();
		this->controller.add(newPanel);
		return newPanel;
	}

	//----------
	shared_ptr<Panels::Groups::Strip> Builder::addStrip() {
		auto newPanel = makeStrip();
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