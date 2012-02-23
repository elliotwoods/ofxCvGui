#include "ofxCvGui/Controller.h"

namespace ofxCvGui {
	extern Assets AssetRegister;

	//----------
	Controller::Controller() {
		this->initialised = false;
	}

	//----------
	void Controller::init(ofPtr<Panels::Groups::Base> rootGroup) {
		ofAddListener(ofEvents.update, this, &Controller::update);
		ofAddListener(ofEvents.draw, this, &Controller::draw);
		ofAddListener(ofEvents.mouseMoved, this, &Controller::mouseMoved);
		ofAddListener(ofEvents.mousePressed, this, &Controller::mousePressed);
		ofAddListener(ofEvents.mouseReleased, this, &Controller::mouseReleased);
		ofAddListener(ofEvents.mouseDragged, this, &Controller::mouseDragged);
		ofAddListener(ofEvents.keyPressed, this, &Controller::keyPressed);	
		ofAddListener(ofEvents.windowResized, this, &Controller::windowResized);

		AssetRegister.init();
		rootGroup->setBounds(ofGetCurrentViewport());
		this->rootGroup = rootGroup;
		this->initialised = true;
	}
	
	//----------
	void Controller::add(PanelPtr& panel) {
		this->rootGroup->add(panel);
	}

	//----------
	void Controller::update(ofEventArgs& args) {
	}

	//----------
	void Controller::draw(ofEventArgs& args) {
		if (!initialised)
			return;
		rootGroup->draw(DrawArguments(ofGetCurrentViewport(), true));
	}

	//----------
	void Controller::mouseMoved(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		rootGroup->mouseAction(MouseArguments(args, MouseMoved, rootGroup->getBounds()));
	}
	
	//----------
	void Controller::mousePressed(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		rootGroup->mouseAction(MouseArguments(args, MousePressed, rootGroup->getBounds()));
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
		if (!initialised)
			return;
		rootGroup->keyboardAction(KeyboardArguments(args, KeyPressed));
	}
	
	//----------
	void Controller::windowResized(ofResizeEventArgs &args) {
		if (!initialised)
			return;
		rootGroup->setBounds(ofRectangle(0,0,ofGetWidth(), ofGetHeight()));
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