#include "ofxCvGui/Controller.h"

namespace ofxCvGui {
	//----------
	Controller::Controller() {
		this->initialised = false;
	}

	//----------
	void Controller::init(PanelPtr& rootPanel) {
		ofAddListener(ofEvents.update, this, &Controller::update);
		ofAddListener(ofEvents.draw, this, &Controller::draw);
		ofAddListener(ofEvents.mouseMoved, this, &Controller::mouseMoved);
		ofAddListener(ofEvents.mousePressed, this, &Controller::mousePressed);
		ofAddListener(ofEvents.mouseReleased, this, &Controller::mouseReleased);
		ofAddListener(ofEvents.mouseDragged, this, &Controller::mouseDragged);
		ofAddListener(ofEvents.keyPressed, this, &Controller::keyPressed);	
		ofAddListener(ofEvents.windowResized, this, &Controller::windowResized);

		this->baseElement = rootPanel;
		this->initialised = true;
	}

	//----------
	void Controller::update(ofEventArgs& args) {
	}

	//----------
	void Controller::draw(ofEventArgs& args) {
		if (!initialised)
			return;
		(*baseElement).draw(DrawArguments(ofGetCurrentViewport(), true));
	}

	//----------
	void Controller::mouseMoved(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		(*baseElement).mouseAction(MouseArguments(args, MouseMoved, (*baseElement).getBounds()));
	}
	
	//----------
	void Controller::mousePressed(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		(*baseElement).mouseAction(MouseArguments(args, MousePressed, (*baseElement).getBounds()));
	}
	
	//----------
	void Controller::mouseReleased(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		(*baseElement).mouseAction(MouseArguments(args, MouseReleased, (*baseElement).getBounds()));
	}
	
	//----------
	void Controller::mouseDragged(ofMouseEventArgs &args) {
		if (!initialised)
			return;
		(*baseElement).mouseAction(MouseArguments(args, MouseDragged, (*baseElement).getBounds()));
	}
	
	//----------
	void Controller::keyPressed(ofKeyEventArgs &args) {
		if (!initialised)
			return;
		(*baseElement).keyboardAction(KeyboardArguments(args, KeyPressed));
	}
	
	//----------
	void Controller::windowResized(ofResizeEventArgs &args) {
		//currently bounds is set through draw
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