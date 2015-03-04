#pragma once

#include "Element.h"

namespace ofxCvGui {
	class ElementSlot : public Element {
	public:
		ElementSlot();
		void setElement(shared_ptr<Element>);
		shared_ptr<Element> getElement() const;
		void clearElement();
	protected:
		shared_ptr<Element> element;
	};
}