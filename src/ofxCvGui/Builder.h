#pragma once

#include "ofBaseTypes.h"
#include "ofPixels.h"

#include "ofxCvGui/Controller.h"

#include "ofxCvGui/Panels/Groups/Grid.h"
#include "ofxCvGui/Panels/Draws.h"
#include "ofxCvGui/Panels/Image.h"
#include "ofxCvGui/Panels/Pixels.h"
#include "ofxCvGui/Panels/PixelsVector.h"
#include "ofxCvGui/Panels/Text.h"
#include "ofxCvGui/Panels/Instructions.h"
#include "ofxCvGui/Panels/Node.h"
#include "ofxCvGui/Panels/Scroll.h"

#include "ofxCvGui/Widgets/Slider.h"
#include "ofxCvGui/Widgets/Spacer.h"

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
		shared_ptr<Panels::Node> add(ofNode & asset, string caption="");
		PanelPtr addBlank(string caption = "");
		shared_ptr<Panels::Node> addWorld(string caption = "");
		shared_ptr<Panels::Scroll> addScroll(string caption = "");
		shared_ptr<Panels::Instructions> addInstructions();
		shared_ptr<Panels::Groups::Grid> addGrid();
		
		//panel factory
		static shared_ptr<Panels::Draws> makePanel(ofBaseDraws& asset, string caption="");
		static shared_ptr<Panels::Image> makePanel(ofImage& asset, string caption="");
		static shared_ptr<Panels::Pixels> makePanel(const ofPixels& asset, string caption="");
		static shared_ptr<Panels::PixelsVector> makePanel(const vector<ofPixels>& asset, string caption="");
		static shared_ptr<Panels::Node> makePanel(ofNode & asset, string caption="");
		static shared_ptr<Panels::Node> makeWorld(string caption = "");
		static shared_ptr<Panels::Scroll> makeScroll(string caption = "");
		static shared_ptr<Panels::Instructions> makeInstructions();
		static shared_ptr<Panels::Groups::Grid> makeGrid();
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