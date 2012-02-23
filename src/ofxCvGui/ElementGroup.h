#pragma once
#include "ofxCvGui/Element.h"

namespace ofxCvGui {
	typedef vector<ElementPtr> ElementSet;
	typedef vector<ElementPtr>::iterator ElementIterator;

	class ElementGroup : public Element {
	public:
		void update();
		void mouseAction(const MouseArguments& mouse);
		void keyboardAction(const KeyboardArguments& keyboard);
	protected:
		void customDraw(const DrawArguments& arguments);
		ElementSet elements;
	};
}