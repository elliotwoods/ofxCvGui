#include "ofxCvGui/Element.h"
namespace ofxCvGui {
	//-----------
	Element::Element() {
		this->enabled = true;
		this->localMouseState = Waiting;
	}

	//-----------
	void Element::update() {
		if (this->enabled) {
			UpdateArguments args;
			onUpdate.notifyListeners(args);
		}
    }
    
    //-----------
	void Element::draw(DrawArguments& arguments) {
		if (this->enabled) {
			ofPushMatrix();
			ofTranslate(bounds.x, bounds.y);
			this->onDraw(arguments);
			ofPopMatrix();
		}
	}
    
    //-----------
    void Element::mouseAction(MouseArguments& args) {
		if (this->enabled) {
			auto localArgs = MouseArguments(args, this->getBounds());
			this->onMouse(localArgs);

			if (localArgs.isLocalPressed()) {
				this->localMouseState = Down;
			} else if (localArgs.action == MouseArguments::Dragged) {
				if (this->localMouseState == Down) {
					this->localMouseState = Dragging;
				}
			} else if (localArgs.action == MouseArguments::Released) {
				if (this->localMouseState == Down) {
					this->onMouseReleased(localArgs);
				}
				this->localMouseState = Waiting;
			}
		}
    }
    
    //-----------
    void Element::keyboardAction(KeyboardArguments &arguments) {
		if (this->enabled) {
			this->onKeyboard(arguments);
		}
    }

    //-----------
	void Element::clearMouseState() {
		this->localMouseState = Waiting;
	}

    //-----------
	Element::LocalMouseState Element::getMouseState() {
		return this->localMouseState;
	}

	//-----------
	void Element::setBounds(const ofRectangle& bounds) {
		if (this->bounds == bounds)
			return;
		this->bounds = bounds;
		this->localBounds = bounds;
		this->localBounds.x = 0;
		this->localBounds.y = 0;
        
		auto arguments = BoundsChangeArguments(this->bounds);
		this->onBoundsChange(arguments);
	}

	//-----------
	void Element::setPosition(const ofVec2f& position) {
		this->bounds.x = position.x;
		this->bounds.y = position.y;
		auto arguments = BoundsChangeArguments(this->bounds);
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
	
	//-----------
	void Element::enable() {
		this->enabled = true;
	}

	//-----------
	void Element::disable() {
		this->enabled = false;
	}
}