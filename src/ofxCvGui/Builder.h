#pragma once

#include "ofBaseTypes.h"
#include "ofPixels.h"

#include "ofxCvGui/Controller.h"
#include "ofxCvGui/Panels/Groups/Grid.h"

#include "ofxCvGui/Panels/Draws.h"
#include "ofxCvGui/Panels/Pixels.h"
#include "ofxCvGui/Panels/PixelsVector.h"

namespace ofxCvGui {
	class Builder {
	public:
		void init();
		void add(ofBaseDraws& asset, string caption="");
		void add(const ofPixels& asset, string caption="");
		void add(const vector<ofPixels>& asset, string caption="");

	protected:
		Controller controller;
		Panels::Groups::Grid* mainGrid;
		PanelPtr rootPanel;
	};
}