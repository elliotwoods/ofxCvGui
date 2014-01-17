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

namespace ofxCvGui {
	class Builder {
	public:
		void init();
		
		//content
		void add(PanelPtr panel);
		ofPtr<Panels::Draws> add(ofBaseDraws& asset, string caption="");
		ofPtr<Panels::Image> add(ofImage& asset, string caption="");
		ofPtr<Panels::Pixels> add(const ofPixels& asset, string caption="");
		ofPtr<Panels::PixelsVector> add(const vector<ofPixels>& asset, string caption="");
		ofPtr<Panels::Node> add(ofNode & asset, string caption="");
		PanelPtr addBlank(string caption = "");
		ofPtr<Panels::Node> addWorld(string caption = "");
		ofPtr<Panels::Scroll> addScroll(string caption = "");
		ofPtr<Panels::Instructions> addInstructions();
		ofPtr<Panels::Groups::Grid> addGrid();
		
		//makers
		static ofPtr<Panels::Draws> makePanel(ofBaseDraws& asset, string caption="");
		static ofPtr<Panels::Image> makePanel(ofImage& asset, string caption="");
		static ofPtr<Panels::Pixels> makePanel(const ofPixels& asset, string caption="");
		static ofPtr<Panels::PixelsVector> makePanel(const vector<ofPixels>& asset, string caption="");
		static ofPtr<Panels::Node> makePanel(ofNode & asset, string caption="");
		static ofPtr<Panels::Node> makeWorld(string caption = "");
		static ofPtr<Panels::Scroll> makeScroll(string caption = "");
		static ofPtr<Panels::Instructions> makeInstructions();
		static ofPtr<Panels::Groups::Grid> makeGrid();
		static ofPtr<Panels::Base> makeBlank();
		

		Controller & getController() { return this->controller; }
		void clear();
		void drop(PanelPtr &panel);

	protected:
		Controller controller;
		Panels::Groups::Grid * mainGrid;
		PanelPtr rootPanel;
	};
}