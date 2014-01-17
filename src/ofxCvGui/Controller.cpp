#include "ofxCvGui/Controller.h"

namespace ofxCvGui {
	//----------
	Controller::Controller() {
		this->initialised = false;
		this->maximised = false;
		this->fullscreen = false;
		this->chromeVisible = true;
	}

	//----------
	void Controller::init(ofPtr<Panels::Groups::Base> rootGroup) {
		ofBackground(30);
		ofSetVerticalSync(true);

		ofAddListener(ofEvents().update, this, &Controller::update);
		ofAddListener(ofEvents().draw, this, &Controller::draw);
		ofAddListener(ofEvents().mouseMoved, this, &Controller::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &Controller::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &Controller::mouseReleased);
		ofAddListener(ofEvents().mouseDragged, this, &Controller::mouseDragged);
		ofAddListener(ofEvents().keyPressed, this, &Controller::keyPressed);	
		ofAddListener(ofEvents().windowResized, this, &Controller::windowResized);

		AssetRegister.init();

		rootGroup->setBounds(ofGetCurrentViewport());
		this->rootGroup = rootGroup;
		this->currentPanel = PanelPtr();
		this->currentPanelBounds = ofGetCurrentViewport();
		this->initialised = true;
	}
	
	//----------
	void Controller::add(PanelPtr& panel) {
		if (!initialised)
			return;
		this->rootGroup->add(panel);
	}

	//----------
	void Controller::drop(PanelPtr& panel) {
		if (!initialised)
			return;
		this->rootGroup->drop(panel);
	}

	//----------
	void Controller::clear() {
		if (!initialised)
			return;
		this->rootGroup->clear();
	}

	//----------
	void Controller::toggleMaximised() {
		//if we were fullscreen, move to simply maximised
		if (this->fullscreen) {
			this->fullscreen = false;
			ofSetFullscreen(false);
			this->maximised = true;
		} else if (this->maximised)  {
			this->maximised = false;
		} else if (this->currentPanel != PanelPtr() ) {
			this->maximised = true;
		} else {
			this->maximised = false;
		}
		auto args = ofResizeEventArgs();
		args.width = ofGetWidth();
		args.height = ofGetHeight();
		this->windowResized(args);
	}

	//----------
	void Controller::toggleFullscreen() {
		if ( this->currentPanel != PanelPtr() )
			this->fullscreen ^= true;
		else
			this->fullscreen = false;
		this->maximised = this->fullscreen;
		ofSetFullscreen(this->fullscreen);
	}
	
	//----------
	void Controller::setFullscreen(PanelPtr panel) {
		this->currentPanel = panel;
		this->currentPanelBounds = ofGetCurrentViewport();
		this->fullscreen = true;
		this->maximised = this->fullscreen;
		ofSetFullscreen(this->fullscreen);
	}

	//----------
	void Controller::showChrome() {
		this->chromeVisible = true;
	}
	
	//----------
	void Controller::hideChrome() {
		this->chromeVisible = false;
	}
	
	//----------
	void Controller::update(ofEventArgs& args) {
		if (!initialised)
			return;
		rootGroup->update();
	}

	//----------
	void Controller::draw(ofEventArgs& args) {
		if (!initialised)
			return;
		if (this->maximised) {
            DrawArguments arg(ofGetCurrentViewport(), ofGetCurrentViewport(), this->chromeVisible);
			this->currentPanel->draw(arg);
		} else {
			//highlight panel
			if (currentPanel != PanelPtr()) {
				ofPushStyle();
				ofEnableAlphaBlending();
				ofSetColor(90, 90, 90, 100);
				ofRect(this->currentPanelBounds);
				ofPopStyle();
			}
            DrawArguments arg(ofGetCurrentViewport(), ofGetCurrentViewport(), this->chromeVisible);
			rootGroup->draw(arg);
            this->rootGroup->onDraw(arg);
		}
	}

	//----------
	PanelGroupPtr Controller::getRootGroup() {
		return this->rootGroup;
	}

	//----------
	void Controller::mouseMoved(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		MouseArguments action(MouseArguments(args, MouseArguments::Moved, rootGroup->getBounds(), this->currentPanel));
		if (this->maximised)
			currentPanel->mouseAction(action);
		else {
			rootGroup->mouseAction(action);
			auto currentPanelBounds = this->rootGroup->getBounds();
			this->currentPanel = PanelPtr(rootGroup->findScreen(ofVec2f(args.x, args.y), currentPanelBounds));
			this->currentPanelBounds = currentPanelBounds;
		}
	}
	
	//----------
	void Controller::mousePressed(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		MouseArguments action(MouseArguments(args, MouseArguments::Pressed, rootGroup->getBounds(), this->currentPanel));
		if (this->maximised)
			currentPanel->mouseAction(action);
		else
			rootGroup->mouseAction(action);
        this->mouseCached = action.global;
	}
	
	//----------
	void Controller::mouseReleased(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		MouseArguments action(args, MouseArguments::Released, rootGroup->getBounds(), this->currentPanel);
        if (this->maximised)
			currentPanel->mouseAction(action);
		else
			rootGroup->mouseAction(action);
	}
	
	//----------
	void Controller::mouseDragged(ofMouseEventArgs &args) {
		if (!initialised)
			return;
        MouseArguments action(args, MouseArguments::Dragged, rootGroup->getBounds(), this->currentPanel, mouseCached);
        if (this->maximised)
			currentPanel->mouseAction(action);
		else
			rootGroup->mouseAction(action);
        this->mouseCached = action.global;
	}
	
	//----------
	void Controller::keyPressed(ofKeyEventArgs &args) {
		if (args.key == 'f')
			this->toggleFullscreen();
		if (args.key == 'm')
			this->toggleMaximised();

		if (!initialised)
			return;

		KeyboardArguments action(args, KeyboardArguments::Pressed, this->currentPanel);
		if (this->maximised)
			currentPanel->keyboardAction(action);
		else
			rootGroup->keyboardAction(action);
	}
	
	//----------
	void Controller::windowResized(ofResizeEventArgs &args) {
		if (!initialised)
			return;
		ofRectangle bounds(0,0,ofGetWidth(), ofGetHeight());
		rootGroup->setBounds(bounds);
		if (this->maximised)
			currentPanel->setBounds(bounds);
	}

	//----------
	bool Controller::checkInitialised() {
		if (this->initialised)
			return true;
		else {
			ofLogError("ofxCvGui") << "cannot perform this action as gui is not intialised";
			return false;
		}
	}
}