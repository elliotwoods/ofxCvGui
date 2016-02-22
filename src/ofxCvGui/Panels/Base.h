#pragma once
#include "../Element.h"
#include "ofRectangle.h"

#define OFXCVGUI_LABEL_PANEL_AND_RETURN newPanel->setCaption(caption); return newPanel;

namespace ofxCvGui {
	namespace Panels {
		class Base;
	}
	typedef shared_ptr<Panels::Base> PanelPtr;

	namespace Panels {
		

		class Base : public Element {
		public:
			Base();
			virtual const shared_ptr<Panels::Base> findScreen(const ofVec2f & xy, ofRectangle & currentPanelBounds) {
				return shared_ptr<Panels::Base>();
			}
			
			ofxLiquidEvent<FilesDraggedArguments> onFilesDragged;
		protected:
			virtual void drawTitle();
		};

		PanelPtr makeBlank(string caption = "");
	}
	typedef vector<PanelPtr> PanelSet;
	typedef vector<PanelPtr>::iterator PanelIterator;
}