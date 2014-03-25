#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Slider : public Element {
		public:
			typedef std::function<void (float &)> Validator; 
			Slider(ofParameter<float> &);
			virtual ~Slider();
			void setValidator(Validator);
			ofxLiquidEvent<ofParameter<float>> onValueChange;
		protected:
			void init();
			void update(UpdateArguments &);
			void draw(DrawArguments &);
			void mouseAction(MouseArguments &);
			void boundsChange(BoundsChangeArguments &);
			float getRangeScale() const;
			void setValue(float);
			void notifyValueChange();

			ofParameter<float> * value;

			float zoom;
			unsigned long startMouseHoldTime;
			float startMouseHoldValue;
			float startMouseHoldMouseX;
			bool mouseHeldOnBar;
			bool mouseHover;

			static ofMesh * tenTicks;
			ofRectangle editBounds;
			Validator validator;
		};
	}
}