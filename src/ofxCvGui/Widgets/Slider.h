#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Slider : public Element {
		public:
			typedef std::function<void(float &)> Validator;
			typedef ofxLiquidEvent<ofParameter<float>>::Functor ValueChangeCallback;

			Slider(ofParameter<float> &);
			Slider(ofParameter<float> &, ValueChangeCallback onValueChange);

			virtual ~Slider();
			void addValidator(Validator);
			void addIntValidator();
			void addStepValidator(float step);
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
			
			float getCheckedValue(float value);
			void checkValueAndNotifyListeners();

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