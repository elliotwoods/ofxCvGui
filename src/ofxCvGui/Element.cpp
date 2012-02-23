#include "ofxCvGui/Element.h"
namespace ofxCvGui {
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

	//-----------
	void Element::setCaption(string caption) {
		this->caption = caption;
	}
}