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
			
			ofxLiquidEvent<FilesDraggedArguments> ofFilesDragged;
		protected:
		};
	}
	typedef shared_ptr<Panels::Base> PanelPtr;
}