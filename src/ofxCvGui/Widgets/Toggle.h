#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Toggle : public Element {
		public:
			Toggle(ofParameter<bool> &);
			Toggle(string caption);
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
			void boundsChange(BoundsChangeArguments &);
			void notifyValueChange();

			bool localAllocation;
			ofParameter<bool> * value;

			ofRectangle buttonBounds;
			bool isMouseDown;
			bool isMouseOver;
		};
	}
}