#pragma once

#include "ofBaseTypes.h"
#include "ofPixels.h"

#include "ofxCvGui/Controller.h"

#include "ofxCvGui/Panels/Groups/Grid.h"
#include "ofxCvGui/Panels/Groups/Strip.h"
#include "ofxCvGui/Panels/Draws.h"
#include "ofxCvGui/Panels/Image.h"
#include "ofxCvGui/Panels/Pixels.h"
#include "ofxCvGui/Panels/PixelsVector.h"
#include "ofxCvGui/Panels/Text.h"
#include "ofxCvGui/Panels/Instructions.h"
#include "ofxCvGui/Panels/World.h"
#include "ofxCvGui/Panels/ElementHost.h"
#include "ofxCvGui/Panels/Widgets.h"
#include "ofxCvGui/Panels/Inspector.h"
#include "ofxCvGui/Panels/Tree.h"

//panels which currently lack factory functions
#include "ofxCvGui/Panels/ElementCanvas.h"
#include "ofxCvGui/Panels/SharedView.h"

namespace ofxCvGui {
	class Builder {
	public:
		void init();
		
		//utility panel adders
		void add(PanelPtr panel);
		shared_ptr<Panels::Draws> add(ofBaseDraws& asset, string caption="");
		shared_ptr<Panels::Image> add(ofImage& asset, string caption="");
		shared_ptr<Panels::Pixels> add(const ofPixels& asset, string caption="");
		shared_ptr<Panels::PixelsVector> add(const vector<ofPixels>& asset, string caption="");
		PanelPtr addBlank(string caption = "");
		shared_ptr<Panels::World> addWorld(string caption = "");
		shared_ptr<Panels::ElementHost> addElementHost(string caption = "");
		shared_ptr<Panels::Widgets> addWidgets(string caption = "");
		shared_ptr<Panels::Instructions> addInstructions();
		shared_ptr<Panels::Inspector> addInspector();
		shared_ptr<Panels::Tree> addTree();
		shared_ptr<Panels::Groups::Grid> addGrid();
		shared_ptr<Panels::Groups::Strip> addStrip();
		
		
		//panel factory
		static shared_ptr<Panels::Draws> makePanel(ofBaseDraws& asset, string caption="");
		static shared_ptr<Panels::Image> makePanel(ofImage& asset, string caption="");
		static shared_ptr<Panels::Pixels> makePanel(const ofPixels& asset, string caption="");
		static shared_ptr<Panels::PixelsVector> makePanel(const vector<ofPixels>& asset, string caption="");
		static shared_ptr<Panels::World> makeWorld(string caption = "");
		static shared_ptr<Panels::ElementHost> makeElementHost(string caption = "");
		static shared_ptr<Panels::Widgets> makeWidgets(string caption = "");
		static shared_ptr<Panels::Instructions> makeInstructions();
		static shared_ptr<Panels::Inspector> makeInspector();
		static shared_ptr<Panels::Tree> makeTree();
		static shared_ptr<Panels::Groups::Grid> makeGrid();
		static shared_ptr<Panels::Groups::Strip> makeStrip();
		static PanelPtr makeBlank();

		Controller & getController() { return this->controller; }
		void clear();
		void drop(PanelPtr &panel);

	protected:
		Controller controller;
		Panels::Groups::Grid * mainGrid;
		PanelPtr rootPanel;
	};
}