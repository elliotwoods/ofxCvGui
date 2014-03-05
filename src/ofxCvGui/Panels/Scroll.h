#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/ElementGroup.h"
#include "../Utils/Utils.h"

namespace ofxCvGui {
	namespace Panels {
		class Scroll : public Base {
		public:
			Scroll();
			void add(ElementPtr);
			ElementGroupPtr getGroup();
			void setScroll(float);
		protected:
			void update();
			void draw(DrawArguments &);
			void mouse(MouseArguments &);
			void keyboard(KeyboardArguments &);
			void arrange();
			float getBarLength() const;
			float getBarY() const;

			ElementGroupPtr elements;
			float position;
			float length;
			bool onScrollBar;
			bool dragTaken;
		};
	}
}