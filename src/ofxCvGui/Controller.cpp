#include "ofxCvGui/Controller.h"

namespace ofxCvGui {
	//----------
	Controller::Controller() {
		this->initialised = false;
		this->maximised = false;
		this->fullscreen = false;
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
			if (this->fullscreen = false)
				ofSetFullscreen(false);
			this->maximised = true;
		} else
			this->maximised ^= true;
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
	void Controller::toggleFullscreen(PanelPtr panel) {
		this->currentPanel = panel;
		this->toggleFullscreen();
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
			this->currentPanel->draw( DrawArguments(ofGetCurrentViewport(), true) );
		} else {
			if (currentPanel != PanelPtr()) {
				ofPushStyle();
				ofEnableAlphaBlending();
				ofSetColor(90, 90, 90, 100);
				ofRect(currentPanel->getBounds());
				ofPopStyle();
			}
			rootGroup->draw(DrawArguments(ofGetCurrentViewport(), true));
		}
	}

	//----------
	void Controller::mouseMoved(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		MouseArguments action(MouseArguments(args, MouseMoved, rootGroup->getBounds()));
		if (this->maximised)
			currentPanel->mouseAction(action);
		else {
			rootGroup->mouseAction(action);
			this->currentPanel = PanelPtr(rootGroup->findScreen( ofVec2f(args.x, args.y) ));
		}
	}
	
	//----------
	void Controller::mousePressed(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		MouseArguments action(MouseArguments(args, MousePressed, rootGroup->getBounds()));
		if (this->maximised)
			currentPanel->mouseAction(action);
		else
			rootGroup->mouseAction(action);
	}
	
	//----------
	void Controller::mouseReleased(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		rootGroup->mouseAction(MouseArguments(args, MouseReleased, rootGroup->getBounds()));
	}
	
	//----------
	void Controller::mouseDragged(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		rootGroup->mouseAction(MouseArguments(args, MouseDragged, rootGroup->getBounds()));
	}
	
	//----------
	void Controller::keyPressed(ofKeyEventArgs &args) {
		if (args.key == 'f')
			this->toggleFullscreen();
		if (args.key == 'm')
			this->toggleMaximised();

		if (!initialised)
			return;

		KeyboardArguments action(args, KeyPressed);
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