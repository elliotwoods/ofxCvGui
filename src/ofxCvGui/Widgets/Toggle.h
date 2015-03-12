#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Toggle : public Element {
		public:
			OFXCVGUI_MAKE_ELEMENT_HEADER(Toggle, ofParameter<bool> & parameter, char hotKey = 0) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Toggle, parameter, hotKey);
			}
			Toggle(ofParameter<bool> &, char hotKey = 0);
			Toggle(string caption, char hotKey = 0);
			Toggle();
			virtual ~Toggle();
			ofxLiquidEvent<ofParameter<bool>> onValueChange;
			void setParameter(ofParameter<bool> &);
			ofParameter<bool> & getParameter();
		protected:
			void init();
			void update(UpdateArguments &);
			void draw(DrawArguments &);
			void mouseAction(MouseArguments &);
			void mouseReleased(MouseArguments &);
			void boundsChange(BoundsChangeArguments &);
			void toggle();
			void notifyValueChange();

			bool localAllocation;
			ofParameter<bool> * value;

			ofRectangle buttonBounds;
			bool isMouseOver;

			char hotKey;
		};

		shared_ptr<Toggle> make(ofParameter<bool> &);
	}
}