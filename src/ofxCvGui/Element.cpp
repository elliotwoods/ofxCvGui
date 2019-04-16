#include "pch_ofxCvGui.h"

using namespace std;

namespace ofxCvGui {
	//-----------
	Element::Element() {
		this->zoomFactor = 1.0f;
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
			DrawArguments localArguments;
			localArguments.chromeEnabled = parentArguments.chromeEnabled;
			localArguments.naturalBounds = this->getBounds();
			localArguments.globalTransform = this->getParentToLocalTransform() * parentArguments.globalTransform;
			localArguments.globalScale = parentArguments.globalScale * this->zoomFactor;
			localArguments.localBounds = ofRectangle(0, 0, localArguments.naturalBounds.width, localArguments.naturalBounds.height);
			localArguments.globalBounds = Utils::operator*(localArguments.localBounds, localArguments.globalTransform);

			//only draw if this Element will be shown on the screen (not outside, not scissored out)
			if (Utils::ScissorManager::X().getScissor().intersects(localArguments.globalBounds)) {

				if (this->cachedView) {
					//--
					//Cached view mechanism
					//--
					//
					if (this->getWidth() != this->cachedView->getWidth()
						|| this->getHeight() != this->cachedView->getHeight()) {
						this->allocateCachedView();
						this->needsViewUpdate = true;
					}

					if (this->needsViewUpdate) {
						bool scissorWasEnabled = Utils::ScissorManager::X().getScissorEnabled();
						Utils::ScissorManager::X().setScissorEnabled(false);

						//if we need to update the view, then redraw the fbo
						this->cachedView->begin();

						ofClear(20, 0);

						//we shouldn't need to do this
						glViewport(0, 0, this->cachedView->getWidth(), this->cachedView->getHeight());

						const auto localBounds = this->getLocalBounds();

						DrawArguments localArgumentsInFbo;
						localArgumentsInFbo.chromeEnabled = localArguments.chromeEnabled;
						localArgumentsInFbo.naturalBounds = localArguments.localBounds;
						localArgumentsInFbo.globalTransform = glm::mat4();
						localArgumentsInFbo.globalScale = 1.0f;
						localArgumentsInFbo.localBounds = localArguments.localBounds;
						localArgumentsInFbo.globalBounds = localArguments.localBounds;

						this->onDraw(localArgumentsInFbo);

						this->cachedView->end();

						if (scissorWasEnabled) {
							Utils::ScissorManager::X().setScissorEnabled(true);
						}

						this->needsViewUpdate = false;
					}
					this->cachedView->draw(this->getBounds());
					//
					//--

				}
				else {

					//--
					//Direct draw mechanism (default)
					//--
					//
					ofPushMatrix();
					ofTranslate(floor(bounds.x), floor(bounds.y));
					ofScale(this->zoomFactor, this->zoomFactor);

					if (this->enableScissor) {
						ofxCvGui::Utils::ScissorManager::X().pushScissor(localArguments.globalBounds);
					}

					this->onDraw(localArguments);

					if (this->enableScissor) {
						ofxCvGui::Utils::ScissorManager::X().popScissor();
					}
					ofPopMatrix();
					//
					//--

				}
			}
		}
	}

	//-----------
	void Element::mouseAction(MouseArguments& parentMouseArguments) {

		// NOTE : This is one of the most important functions of this entire addon

		if (this->enabled) {
			auto parentToLocalTransform = this->getParentToLocalTransform();

			MouseArguments localMouseArguments = parentMouseArguments;
			localMouseArguments.local = glm::inverse(parentToLocalTransform) * glm::vec4(parentMouseArguments.local, 0.0f, 1.0f);
			localMouseArguments.localNormalized = localMouseArguments.local / glm::vec2(this->getWidth(), this->getHeight());
			localMouseArguments.movement = parentMouseArguments.movement / glm::vec2(parentToLocalTransform[0][0], parentToLocalTransform[1][1]);

			this->mouseOver = localMouseArguments.isLocal();

			//some elements might not want to perform a local hit test since their bounds are meaningless (e.g. ElementSlot)
			auto isHit = this->mouseOver || !this->enableHitTestOnBounds;

			switch (parentMouseArguments.action) {
			case MouseArguments::Action::Pressed:
				{
					//special case for pressed, only pass if local and not already taken
					if (isHit && !localMouseArguments.isTaken()) {
						//pass the mouse event onto Element's event handlers
						this->onMouse.notifyListenersInReverse(localMouseArguments);

						//if we have a mouse released handler, then we must also try and take the click
						if (!this->onMouseReleased.empty()) {
							localMouseArguments.takeMousePress(this);
						}

						//check if one of the handlers took ownership of the mouse down event
						if (localMouseArguments.isTaken()) {
							// if this element or any nested elements took the click, notify upstream arguments of the take
							auto clickOwner = localMouseArguments.getOwner();
							parentMouseArguments.forceMouseTake(clickOwner); // if this element took, notify upstream

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
				}
				break;


			case MouseArguments::Action::Dragged:
				{
					//if mouse is being dragged and went down in this Element, then we're now dragging
					if (this->localMouseState == LocalMouseState::Down) {
						this->localMouseState = LocalMouseState::Dragging;
					}

					//if our local state is dragging or child owns the mouse, then pass through the drag action
					//this is also true when a child element is being dragged
					//to check if this element is being dragged, use args.isDragging(myElement) inside your handler
					if (this->localMouseState & (LocalMouseState::Dragging | LocalMouseState::ChildOwnsMouse)) {
						this->onMouse(localMouseArguments);
					}
				}
				break;


			case MouseArguments::Action::Released:
				{
					//only trigger the onMouseReleased if the mouse release was local
					if (this->mouseOver && (this->localMouseState & (LocalMouseState::Dragging | LocalMouseState::Down))) {
						this->onMouseReleased(localMouseArguments);
					}

					//either way, we need to pass this into the Element's handlers and children for consideration unless we and no children owns the mouse
					if (this->localMouseState != LocalMouseState::Waiting) {
						this->onMouse(localMouseArguments);
						this->localMouseState = LocalMouseState::Waiting;
					}
				}
				break;


			case MouseArguments::Action::DoubleClick:
				{
					this->onMouse(localMouseArguments);
				}
				break;


			case MouseArguments::Action::Moved:
				{
					this->onMouse(localMouseArguments);
				}
				break;


			default:
				break;
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
		return this->getMouseState() == LocalMouseState::Down || (this->getMouseState() == LocalMouseState::Dragging && this->isMouseOver());
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
		this->updateParentToLocalTransform();

		if (this->cachedView) {
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
	void Element::setPosition(const glm::vec2 & position) {
		auto newBounds = this->getBounds();
		bounds.x = position.x;
		bounds.y = position.y;
		this->setBounds(this->bounds); // best to always let setBounds deal with this. even if it means sometimes reallocating
	}

	//-----------
	const ofRectangle & Element::getBounds() const {
		return this->bounds;
	}

	//-----------
	ofRectangle Element::getLocalBounds() const {
		auto localBounds = this->bounds;
		localBounds.x = 0.0f;
		localBounds.y = 0.0f;
		return localBounds;
	}

	//-----------
	ofRectangle Element::getBoundsInParent() const {
		return Utils::operator*(this->getLocalBounds(), this->getParentToLocalTransform());
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
	void Element::setZoom(float zoom) {
		ZoomChangeArguments zoomChangeArguments = { this->zoomFactor, zoom };

		this->zoomFactor = zoom;
		this->updateParentToLocalTransform();

		this->onZoomChange(zoomChangeArguments);
	}

	//-----------
	float Element::getZoom() const {
		return this->zoomFactor;
	}

	//-----------
	glm::mat4x4 Element::getParentToLocalTransform() const {
		return this->parentToLocalTransform;
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
	void Element::setScissorEnabled(bool enableScissor) {
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


	//----------
	void Element::addChild(ElementPtr child) {
		if (this->children.find(child) == this->children.end()) {
			this->children.insert(child);
			child->addListenersToParent(this);
		}
	}


	//----------
	void Element::removeChild(ElementPtr child) {
		auto findChild = this->children.find(child);
		if (findChild != this->children.end()) {
			child->removeListenersFromParent(this);
			this->children.erase(findChild);
		}
	}


	//----------
	set<ofxCvGui::ElementPtr> Element::getChildren() const {
		return this->children;
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
	void Element::updateParentToLocalTransform() {
		this->parentToLocalTransform = glm::scale(glm::vec3(this->zoomFactor
			, this->zoomFactor
			, 1.0f))
			
			* glm::translate(this->getBounds().getTopLeft());
	}

	//-----------
	void Element::allocateCachedView() {
		ofFbo::Settings fboSettings;
		fboSettings.width = (int) this->bounds.width;
		fboSettings.height = (int) this->bounds.height;
		fboSettings.internalformat = GL_RGBA;
		fboSettings.numSamples = 0;

		this->cachedView->allocate(fboSettings);
	}

	//-----------
	void Element::setHitTestOnBounds(bool enableHitTestOnBounds) {
		this->enableHitTestOnBounds = enableHitTestOnBounds;
	}

	//-----------
	ElementPtr makeElement() {
		return make_shared<Element>();
	}
}