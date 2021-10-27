#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/ElementGroup.h"
#include "../Utils/Utils.h"

namespace ofxCvGui {
	namespace Panels {
		class OFXCVGUI_API_ENTRY Scroll : public Base {
		public:
			Scroll();
			void add(ElementPtr);

			template<typename ElementType,
				typename = std::enable_if<std::is_base_of<Element, ElementType>::value> >
			shared_ptr<ElementType> add(ElementType * element) {
				auto newElement = shared_ptr<ElementType>(element);
				this->add(newElement);
				return newElement;
			}

			void clear();
			ElementGroupPtr getElementGroup();

			///Scroll functions
			/// Note : Scroll is positive as you scroll down (first item is off the top)
			///{
			void setScroll(float);
			float getScroll() const;
			void scrollToInclude(ElementPtr);
			float getLength() const;
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
			float position = 0.0f;
			float length = 0.0f;
			bool onScrollBar = false;
		};
	}
}