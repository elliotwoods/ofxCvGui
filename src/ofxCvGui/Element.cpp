#include "ofxCvGui/Element.h"
namespace ofxCvGui {
	//-----------
	Element::Element() {
		this->enabled = true;
		this->enableScissor = false;
		this->localMouseState = Waiting;
		this->mouseOver = false;
		this->enableHitTestOnBounds = true;
		this->cachedView = nullptr;
		this->needsViewUpdate = false;
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
			if (this->cachedView) {
				if (this->needsViewUpdate) {
					//if we need to update the view, then redraw the fbo
					this->cachedView->begin();
					ofClear(80, 0);

					bool scissorWasEnabled = Utils::disableScissor();

					const auto localBounds = this->getLocalBounds();
					DrawArguments localArguments(localBounds, localBounds, parentArguments.chromeEnabled);
					this->onDraw(localArguments);
					
					if (scissorWasEnabled) {
						Utils::enableScissor();
					}

					this->cachedView->end();
					this->needsViewUpdate = false;
				}
				this->cachedView->draw(this->bounds);
			} else {
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
	}
    
    //-----------
    void Element::mouseAction(MouseArguments& args) {
		if (this->enabled) {
			auto localArgs = MouseArguments(args, this->getBounds());
			this->mouseOver = localArgs.isLocal();

			if(args.action == MouseArguments::Pressed) {
				//some elements might not want to perform a local hit test since their bounds are meaningless (e.g. ElementSlot)
				auto isHit = localArgs.isLocal() || !this->enableHitTestOnBounds;

				//special case for pressed, only pass if local and not already taken
				if (isHit && !localArgs.isTaken()) {
					//pass the mouse event onto Element's event handlers
					this->onMouse.notifyListenersInReverse(localArgs);

					//if we have a mouse released handler, then we must also try and take the click
					if (!this->onMouseReleased.empty()) {
						localArgs.takeMousePress(this);
					}

					//check if one of the handlers took ownership of the mouse down event
					if (localArgs.isTaken()) {
						// if this element or any nested elements took the click, notify upstream arguments of the take
						auto clickOwner = localArgs.getOwner();
						args.forceMouseTake(clickOwner); // if this element took, notify upstream

						if (clickOwner == this) {
							// if we're the ones who took the mouse down action, let's flag it in our local state
							this->localMouseState = LocalMouseState::Down;
						}
						else {
							// otherwise a child element handled by this element has taken ownership of the mouse
							this->localMouseState = LocalMouseState::ChildOwnsMouse;
						}
					}
				}
			} else if (args.action == MouseArguments::Dragged) {
				//if mouse is being dragged and went down in this Element, then we're now dragging
				if (this->localMouseState == LocalMouseState::Down) {
					this->localMouseState = LocalMouseState::Dragging;
				}

				//if our local state is dragging or child owns the mouse, then pass through the drag action
				//this is also true when a child element is being dragged
				//to check if this element is being dragged, use args.isDragging(myElement) inside your handler
				if (this->localMouseState & (LocalMouseState::Dragging | LocalMouseState::ChildOwnsMouse)) {
					this->onMouse(localArgs);
				}
			} else if (args.action == MouseArguments::Released) {
				//if we've got the cursor, then let's take the action and release the cursor
				if (this->localMouseState != LocalMouseState::Waiting) {
					//only trigger the onMouseReleased if the mouse release was local
					if (this->mouseOver && (this->localMouseState & (LocalMouseState::Dragging | LocalMouseState::Down))) {
						this->onMouseReleased(localArgs);
					}
				}
				//either way, we need to pass this into the Element's handlers and children for consideration
				this->onMouse(localArgs);
				this->localMouseState = LocalMouseState::Waiting;
			} else {
				//mouse moved
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

		if (this->cachedView) {
			this->allocateCachedView();
			this->markViewDirty();
		}

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
	const string & Element::getCaption() const {
		return this->caption;
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
	void Element::setScissor(bool enableScissor) {
		this->enableScissor = enableScissor;
	}

	//-----------
	void Element::addListenersToParent(Element * parent, bool syncBoundsToParent) {
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
		if (syncBoundsToParent) {
			//resize to parents bounds
			parent->onBoundsChange.addListener([this](BoundsChangeArguments & args) {
				this->setBounds(args.localBounds);
			}, this);
		}
		else {
			//just rearrange ourselves (e.g. we just exited a fullscreen state)
			parent->onBoundsChange.addListener([this](BoundsChangeArguments &) {
				this->arrange();
			}, this);
		}
	}

	//-----------
	void Element::addListenersToParent(shared_ptr<Element> parent, bool syncBoundsToParent) {
		if (parent) {
			this->addListenersToParent(parent.get(), syncBoundsToParent);
		}
	}

	//-----------
	void Element::removeListenersFromParent(Element * parent) {
		parent->onUpdate.removeListeners(this);
		parent->onDraw.removeListeners(this);
		parent->onMouse.removeListeners(this);
		parent->onKeyboard.removeListeners(this);
		parent->onBoundsChange.removeListeners(this);
	}

	//-----------
	void Element::removeListenersFromParent(shared_ptr<Element> parent) {
		if (parent) {
			this->removeListenersFromParent(parent.get());
		}
	}

	//-----------
	void Element::setCachedView(bool cachedViewEnabled) {
		if (cachedViewEnabled) {
			if (!this->cachedView) {
				this->cachedView = new ofFbo();
				this->allocateCachedView();
				this->markViewDirty();
			}
		}
		else {
			if (this->cachedView) {
				delete this->cachedView;
			}
		}
	}

	//-----------
	void Element::markViewDirty() {
		this->needsViewUpdate = true;
	}

	//-----------
	void Element::allocateCachedView() {
		ofFbo::Settings fboSettings;
		fboSettings.width = this->bounds.width;
		fboSettings.height = this->bounds.height;
		fboSettings.internalformat = GL_RGBA;
		//fboSettings.numSamples = 2;

		this->cachedView->allocate(fboSettings);
	}

	//-----------
	void Element::setHitTestOnBounds(bool enableHitTestOnBounds) {
		this->enableHitTestOnBounds = enableHitTestOnBounds;
	}
}