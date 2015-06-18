#pragma once
#include "../Element.h"
#include "ofRectangle.h"

namespace ofxCvGui {
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
	}
	typedef shared_ptr<Panels::Base> PanelPtr;
	typedef vector<PanelPtr> PanelSet;
	typedef vector<PanelPtr>::iterator PanelIterator;
}