#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Slider : public Element {
		public:
			Slider(const string & caption, float & value, const float minimum, const float maximum);
			Slider(ofParameter<float> &);
			virtual ~Slider();
		protected:
			enum MouseHoldState {
				None = 0,
				Hold,
				Dragged
			};

			void init();
			void update(UpdateArguments &);
			void draw(DrawArguments &);
			void mouseAction(MouseArguments &);
			void boundsChange(BoundsChangeArguments &);

			ofParameter<float> * value;
			bool valueLocallyAllocated;

			bool zoom;
			unsigned long startMouseHoldTime;
			float startMouseHoldX;
			MouseHoldState mouseHoldState;

			static ofMesh * ticks;
			ofRectangle editBounds;
		};
	}
}