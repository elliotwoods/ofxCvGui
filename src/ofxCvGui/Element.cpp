#include "ofxCvGui/Element.h"
namespace ofxCvGui {
	//-----------
	void Element::update() {
        UpdateArguments args;
        ofNotifyEvent(this->onUpdate, args, this);
    }
    
    //-----------
	void Element::draw(const DrawArguments& arguments) {
		ofPushMatrix();
		ofTranslate(bounds.x, bounds.y);
        
		ofNotifyEvent(this->onDraw, const_cast<DrawArguments&>(arguments), this);
		
        ofPopMatrix();
	}
    
    //-----------
    void Element::mouseAction(const MouseArguments &mouse) {
        ofNotifyEvent(this->onMouseAction, const_cast<MouseArguments&>(mouse), this);
    }
    
    //-----------
    void Element::keyboardAction(const KeyboardArguments &key) {
        ofNotifyEvent(this->onKeyboardAction, const_cast<KeyboardArguments&>(key), this);
    }

	//-----------
	void Element::setBounds(const ofRectangle& bounds) {
		if (this->bounds == bounds)
			return;
		this->bounds = bounds;
		this->localBounds = bounds;
		this->localBounds.x = 0;
		this->localBounds.y = 0;
        
        ofNotifyEvent(this->onBoundsChange, localBounds, this);
	}

	//-----------
	const ofRectangle& Element::getBounds() const{
		return this->bounds;
	}

	//-----------
	float Element::getWidth() const {
		return this->getBounds().width;;
	}

	//-----------
	float Element::getHeight() const{
		return this->getBounds().height;
	}

	//-----------
	void Element::setCaption(string caption) {
		this->caption = caption;
	}
}