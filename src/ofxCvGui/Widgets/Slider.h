#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Slider : public Element {
		public:
			typedef std::function<void (float &)> Validator; 

			OFXCVGUI_MAKE_ELEMENT_HEADER(Slider, ofParameter<float> & parameter) {
				OFXCVGUI_MAKE_ELEMENT_BODY(Slider, parameter);
			}
			Slider(ofParameter<float> &);
			virtual ~Slider();
			void addValidator(Validator);
			void addIntValidator();
			void clearValidators();
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
			bool mouseWentDownOnSlider;
			bool mouseHover;

			static ofMesh * tenTicks;
			ofRectangle editBounds;
			vector<Validator> validators;
		};
	}
}