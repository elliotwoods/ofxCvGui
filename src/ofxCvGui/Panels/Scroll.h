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
			virtual void clear();
			ElementGroupPtr getElementGroup();

			///Scroll functions
			/// Note : Scroll is positive as you scroll down (first item is off the top)
			///{
			void setScroll(float);
			float getScroll() const;
			void scrollToInclude(ElementPtr);
			///{
		protected:
			void update();
			void draw(DrawArguments &);
			void mouse(MouseArguments &);
			void keyboard(KeyboardArguments &);
			void arrangeScroll();
			float getBarLength() const;
			float getBarY() const;

			ElementGroupPtr elements;
			float position;
			float length;
			bool onScrollBar;
		};
	}
}