#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Toggle : public Element {
		public:
			Toggle(ofParameter<bool> &, char hotKey = 0);
			Toggle(std::string caption, char hotKey = 0);
			Toggle(std::string caption, function<bool()> get, function<void(bool)> set, char hotKey = 0);
			Toggle(); // you must call setParameter later

			virtual ~Toggle();
			ofxLiquidEvent<ofParameter<bool>> onValueChange;
			void setParameter(ofParameter<bool> &);
			ofParameter<bool> & getParameter();

			void setHotKey(char);
			char getHotKey() const;
		protected:
			void init();
			void update(UpdateArguments &);
			void draw(DrawArguments &);
			void mouseAction(MouseArguments &);
			void toggle();
			void notifyValueChange();

			bool localAllocation = false;
			ofParameter<bool> * value;

			char hotKey = 0;
		};
	}
}