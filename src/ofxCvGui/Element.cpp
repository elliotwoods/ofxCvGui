#include "ofxCvGui/Element.h"
namespace ofxCvGui {
#pragma mark Element
	//-----------
	void Element::draw(const DrawArguments& arguments) {
		ofPushMatrix();
		ofTranslate(bounds.x, bounds.y);
		this->customDraw(arguments);
		ofPopMatrix();
	}

	//-----------
	void Element::setBounds(const ofRectangle& bounds) {
		if (this->bounds == bounds)
			return;
		this->bounds = bounds;
		boundsChange();
	}
	//-----------
	const ofRectangle& Element::getBounds() {
		return this->bounds;
	}

#pragma mark ElementPtr
	//-----------
	ElementPtr::ElementPtr() {
		this->element = 0;
		this->localAllocation = false;
	}

	//-----------
	ElementPtr::ElementPtr(const ElementPtr& other) {
		this->element = other.element;
		this->localAllocation = false;
	}

	//-----------
	ElementPtr::~ElementPtr() {
		delete element;
	}
	
	//-----------
	Element& ElementPtr::operator*() {
		return *element;
	}
	
	//-----------
	ElementPtr& ElementPtr::operator=(Element* element) {
		this->element = element;
		localAllocation = true;
		return *this;
	}
}