#pragma once

#include "ofBaseTypes.h"
#include "ofPixels.h"

#include "ofxCvGui/Controller.h"
#include "ofxCvGui/Panels/Groups/Grid.h"

#include "ofxCvGui/Panels/Draws.h"
#include "ofxCvGui/Panels/Pixels.h"
#include "ofxCvGui/Panels/PixelsVector.h"

#include "ofxCvGui/Panels/Text.h"
#include "ofxCvGui/Panels/Instructions.h"

namespace ofxCvGui {
	class Builder {
	public:
		void init();
		PanelPtr add(ofBaseDraws& asset, string caption="");
		PanelPtr add(const ofPixels& asset, string caption="");
		PanelPtr add(const vector<ofPixels>& asset, string caption="");
		PanelPtr addInstructions();

		void drop(PanelPtr &panel);

	protected:
		Controller controller;
		Panels::Groups::Grid* mainGrid;
		PanelPtr rootPanel;
	};
}