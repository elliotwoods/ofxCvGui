#include "ofxCvGui/Element.h"
namespace ofxCvGui {
	//-----------
	Element::Element() {
		this->enabled = true;
		this->enableScissor = false;
		this->localMouseState = Waiting;
		this->mouseOver = false;
	}

	//-----------
	void Element::update() {
		if (this->enabled) {
			UpdateArguments args;
			onUpdate.notifyListeners(args);
		}
    }
    
    //-----------
	void Element::draw(const DrawArguments& parentArguments) {
		if (this->enabled) {
			auto boundsWithinParent = this->getBounds();
			ofRectangle globalBounds = boundsWithinParent;
			globalBounds.x += parentArguments.globalBounds.x;
			globalBounds.y += parentArguments.globalBounds.y;
			DrawArguments localArguments(boundsWithinParent, globalBounds, parentArguments.chromeEnabled);

			ofPushMatrix();
			ofTranslate(bounds.x, bounds.y);
			if (this->enableScissor) {
				ofxCvGui::Utils::pushScissor(localArguments.globalBounds);
			}
			this->onDraw(localArguments);
			if (this->enableScissor) {
				ofxCvGui::Utils::popScissor();
			}
			ofPopMatrix();
		}
	}
    
    //-----------
    void Element::mouseAction(MouseArguments& args) {
		if (this->enabled) {
			auto localArgs = MouseArguments(args, this->getBounds());
			this->mouseOver = localArgs.isLocal();

			if(args.action == MouseArguments::Pressed) {
				//special case for pressed, only pass if local
				if (localArgs.isLocal()) {
					this->onMouse(localArgs);
					if (localArgs.isTaken()) {
						auto clickOwner = localArgs.getOwner();
						args.forceMouseTake(clickOwner); // if this element took, notify upstream
						this->localMouseState = LocalMouseState::Down;
					}
				}
			} else if (args.action == MouseArguments::Dragged) {
				//if mouse is being dragged and is taken by this element, then set local state to dragging
				if (this->localMouseState == LocalMouseState::Down) {
					this->localMouseState = LocalMouseState::Dragging;
				}
				//if our local state is dragging then pass through the drag action
				if (this->localMouseState == LocalMouseState::Dragging) {
					this->onMouse(localArgs);
				}
			} else if (args.action == MouseArguments::Released) {
				//if we've got the cursor, then let's take the action and release the cursor
				if (this->localMouseState != LocalMouseState::Waiting) {
					//only pass the event into the element if the mouse release was local
					if (this->mouseOver) {
						this->onMouseReleased(localArgs);
					}
					this->onMouse(localArgs);
					this->localMouseState = LocalMouseState::Waiting;
				}
			} else {
				this->onMouse(localArgs);
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
	Element::LocalMouseState Element::getMouseState() const {
		return this->localMouseState;
	}

	//-----------
	bool Element::isMouseDown() const {
		return this->getMouseState() == LocalMouseState::Down;
	}

	//-----------
	bool Element::isMouseDragging() const {
		return this->getMouseState() == LocalMouseState::Dragging;
	}

	//-----------
	bool Element::isMouseOver() const {
		return this->mouseOver;
	}

	//-----------
	void Element::setBounds(const ofRectangle& bounds) {
		this->bounds = bounds;
		this->localBounds = bounds;
		this->localBounds.x = 0;
		this->localBounds.y = 0;
        
		auto arguments = BoundsChangeArguments(this->bounds);
		this->onBoundsChange(arguments);
	}

	//-----------
	void Element::arrange() {
		auto arguments = BoundsChangeArguments(this->bounds);
		this->onBoundsChange(arguments);
	}

	//-----------
	void Element::setWidth(float width) {
		auto bounds = this->getBounds();
		bounds.setWidth(width);
		this->setBounds(bounds);
	}

	//-----------
	void Element::setHeight(float height) {
		auto bounds = this->getBounds();
		bounds.setHeight(height);
		this->setBounds(bounds);
	}

	//-----------
	void Element::setPosition(const ofVec2f& position) {
		this->bounds.x = position.x;
		this->bounds.y = position.y;
		auto arguments = BoundsChangeArguments(this->bounds);
		this->onBoundsChange(arguments);
	}

	//-----------
	const ofRectangle & Element::getBounds() const {
		return this->bounds;
	}

	//-----------
	const ofRectangle Element::getLocalBounds() const {
		auto localBounds = this->bounds;
		localBounds.x = 0.0f;
		localBounds.y = 0.0f;
		return localBounds;
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
		this->onCaptionChange(caption);
	}
	
	//-----------
	void Element::setEnabled(bool enabled) {
		this->enabled = enabled;
	}

	//-----------
	bool Element::getEnabled() const {
		return this->enabled;
	}

	//-----------
	void Element::enable() {
		this->enabled = true;
	}

	//-----------
	void Element::disable() {
		this->enabled = false;
	}

	//-----------
	void Element::addListenersToParent(Element * parent) {
		parent->onUpdate.addListener([this](UpdateArguments & args) {
			this->update();
		}, this);
		parent->onDraw.addListener([this](DrawArguments & args) {
			this->draw(args);
		}, this);
		parent->onMouse.addListener([this](MouseArguments & args) {
			this->mouseAction(args);
		}, this);
		parent->onKeyboard.addListener([this](KeyboardArguments & args) {
			this->keyboardAction(args);
		}, this);
	}

	//-----------
	void Element::addListenersToParent(shared_ptr<Element> parent) {
		if (parent) {
			this->addListenersToParent(parent);
		}
	}

	//-----------
	void Element::removeListenersFromParent(Element * parent) {
		parent->onUpdate.removeListeners(this);
		parent->onDraw.removeListeners(this);
		parent->onMouse.removeListeners(this);
		parent->onKeyboard.removeListeners(this);
	}

	//-----------
	void Element::removeListenersFromParent(shared_ptr<Element> parent) {
		if (parent) {
			this->removeListenersFromParent(parent.get());
		}
	}

	//-----------
	void Element::setScissor(bool enableScissor) {
		this->enableScissor = enableScissor;
	}
}