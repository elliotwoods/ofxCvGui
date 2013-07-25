#include "ofxCvGui/Element.h"
namespace ofxCvGui {
	//-----------
	void Element::update() {
        UpdateArguments args;
		onUpdate.notifyListeners(args);
    }
    
    //-----------
	void Element::draw(DrawArguments& arguments) {
		ofPushMatrix();
		ofTranslate(bounds.x, bounds.y);
        this->onDraw(arguments);
        ofPopMatrix();
	}
    
    //-----------
    void Element::mouseAction(MouseArguments& arguments) {
		this->onMouse(arguments);
    }
    
    //-----------
    void Element::keyboardAction(KeyboardArguments &arguments) {
		this->onKeyboard(arguments);
    }

	//-----------
	void Element::setBounds(const ofRectangle& bounds) {
		if (this->bounds == bounds)
			return;
		this->bounds = bounds;
		this->localBounds = bounds;
		this->localBounds.x = 0;
		this->localBounds.y = 0;
        
		auto arguments = BoundsChangeArguments(bounds);
		this->onBoundsChange(arguments);
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