#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Slider : public Element {
		public:
			Slider(ofParameter<float> &);
			virtual ~Slider();
		protected:
			void init();
			void update(UpdateArguments &);
			void draw(DrawArguments &);
			void mouseAction(MouseArguments &);
			void boundsChange(BoundsChangeArguments &);
			float getRangeScale() const;

			ofParameter<float> * value;

			float zoom;
			unsigned long startMouseHoldTime;
			float startMouseHoldValue;
			float startMouseHoldMouseX;
			bool mouseHeldOnBar;

			static ofMesh * ticks;
			ofRectangle editBounds;
		};
	}
}