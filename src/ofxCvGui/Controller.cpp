#include "ofxCvGui/Controller.h"
#include "ofxAssets.h"

namespace ofxCvGui {
	//----------
	Controller::Controller() {
		this->initialised = false;
		this->maximised = false;
		this->fullscreen = false;
		this->chromeVisible = true;

		this->cachedWidth = 0.0f;
		this->cachedHeight = 0.0f;
	}

	//----------
	void Controller::init(shared_ptr<Panels::Groups::Base> rootGroup) {
		ofBackground(30);

		ofAddListener(ofEvents().update, this, &Controller::update);
		ofAddListener(ofEvents().draw, this, &Controller::draw);
		ofAddListener(ofEvents().mouseMoved, this, &Controller::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &Controller::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &Controller::mouseReleased);
		ofAddListener(ofEvents().mouseDragged, this, &Controller::mouseDragged);
		ofAddListener(ofEvents().keyPressed, this, &Controller::keyPressed);	
		ofAddListener(ofEvents().windowResized, this, &Controller::windowResized);
		ofAddListener(ofEvents().fileDragEvent, this, &Controller::filesDragged);

#ifdef __DEBUGGING__
		//if we're still debugging in the build location, copy in latest assets
		{
			auto checkDir =	ofDirectory("../../../../../addons/ofxCvGui2/data/assets/");
			if (checkDir.exists()) {
				checkDir.copyTo(".");
			}
		}
#endif
		ofxAssets::AssetRegister.addAddon("ofxCvGui");
		
		rootGroup->setBounds(ofGetCurrentViewport());
		this->rootGroup = rootGroup;
		this->currentPanel = PanelPtr();
		this->currentPanelBounds = ofGetCurrentViewport();
		this->initialised = true;
	}
	
	//----------
	void Controller::add(PanelPtr panel) {
		if (!initialised)
			return;
		this->rootGroup->add(panel);
	}

	//----------
	void Controller::drop(PanelPtr panel) {
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
		if (this->maximised) {
			this->currentPanel->setBounds(ofGetCurrentViewport());
		} else {
			this->rootGroup->setBounds(ofGetCurrentViewport());
		}
	}

	//----------
	void Controller::toggleFullscreen() {
		if (this->currentPanel != PanelPtr())
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
		if (cachedWidth != ofGetWidth() || cachedHeight != ofGetHeight()) {
			//on windows the event doesn't always fire
			ofResizeEventArgs args;
			args.width = ofGetWidth();
			args.height = ofGetHeight();
			this->windowResized(args);
		}
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
				ofSetColor(40, 40, 40, 100);
				ofRect(this->currentPanelBounds);
				ofPopStyle();
			}
            DrawArguments arg(ofGetCurrentViewport(), ofGetCurrentViewport(), this->chromeVisible);
			this->rootGroup->draw(arg);
		}
	}

	//----------
	PanelGroupPtr Controller::getRootGroup() {
		return this->rootGroup;
	}

	//----------
	PanelPtr Controller::getPanelUnderCursor(const ofVec2f & position) {
		if (this->maximised) {
			return currentPanel;
		} else {
			ofRectangle panelBounds = this->rootGroup->getBounds();
			return this->findPanelUnderCursor(panelBounds, position);
		}
	}

	//----------
	void Controller::mouseMoved(ofMouseEventArgs & args) {
		if (!initialised)
			return;
		MouseArguments action(MouseArguments(args, MouseArguments::Moved, rootGroup->getBounds(), this->currentPanel));
		if (this->maximised)
			currentPanel->mouseAction(action);
		else {
			rootGroup->mouseAction(action);
			this->updateCurrentPanel();
		}
	}
	
	//----------
	void Controller::mousePressed(ofMouseEventArgs & args) {
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
	void Controller::mouseReleased(ofMouseEventArgs & args) {
		if (!initialised)
			return;
		MouseArguments action(args, MouseArguments::Released, rootGroup->getBounds(), this->currentPanel);
        if (this->maximised)
			currentPanel->mouseAction(action);
		else
			rootGroup->mouseAction(action);
	}
	
	//----------
	void Controller::mouseDragged(ofMouseEventArgs & args) {
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
	void Controller::keyPressed(ofKeyEventArgs & args) {
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
	void Controller::windowResized(ofResizeEventArgs & args) {
		if (!initialised)
			return;
		ofRectangle bounds(0,0,ofGetWidth(), ofGetHeight());
		rootGroup->setBounds(bounds);
		if (this->maximised) {
			currentPanel->setBounds(bounds);
		}
		updateCurrentPanel();
		this->cachedWidth = args.width;
		this->cachedHeight = args.height;
	}

	//----------
	void Controller::filesDragged(ofDragInfo & args) {
		if (!initialised)
			return;
		auto rootBounds = this->rootGroup->getBounds();
		auto panel = this->findPanelUnderCursor(rootBounds);
		if (panel != PanelPtr()) {
			auto panelBounds = panel->getBounds();
			ofVec2f panelTopLeft = panelBounds.getTopLeft();
			auto newArgs = FilesDraggedArguments((ofVec2f) args.position - panelTopLeft, (ofVec2f) args.position, args.files);
			panel->ofFilesDragged(newArgs);
		}
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

	//----------
	PanelPtr Controller::findPanelUnderCursor(ofRectangle & panelBounds, const ofVec2f & position) {
		return PanelPtr(rootGroup->findScreen(position, panelBounds));
	}

	//----------
	void Controller::updateCurrentPanel() {
		if (!this->maximised) {
			auto currentPanelBounds = this->rootGroup->getBounds();
			this->currentPanel = this->findPanelUnderCursor(currentPanelBounds);
			this->currentPanelBounds = currentPanelBounds;
		}
	}
}