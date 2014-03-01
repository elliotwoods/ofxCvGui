#pragma once
#include "../Element.h"
#include "ofParameter.h"

namespace ofxCvGui {
	namespace Widgets {
		class Slider : public Element {
		public:
			Slider(const string & caption, float & value, const float minimum, const float maximum);
			Slider(ofParameter<float> &);
		protected:
			void update(UpdateArguments &);
			void draw(DrawArguments &);
			void mouseAction(MouseArguments &);

			bool zoom;
		};
	}
}