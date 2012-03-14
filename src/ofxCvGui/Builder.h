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

#include "ofxCvGui/Panels/Node.h"

namespace ofxCvGui {
	class Builder {
	public:
		void init();
		ofPtr<Panels::Draws> add(ofBaseDraws& asset, string caption="");
		ofPtr<Panels::Pixels> add(const ofPixels& asset, string caption="");
		ofPtr<Panels::PixelsVector> add(const vector<ofPixels>& asset, string caption="");
		ofPtr<Panels::Node> add(ofNode & asset, string caption="");
		ofPtr<Panels::Instructions> addInstructions();

		void clear();
		void drop(PanelPtr &panel);

	protected:
		Controller controller;
		Panels::Groups::Grid* mainGrid;
		PanelPtr rootPanel;
	};
}