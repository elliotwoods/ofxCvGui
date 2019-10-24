#include "pch_ofxCvGui.h"

//----------
OFXSINGLETON_DEFINE(ofxCvGui::Controller);

namespace ofxCvGui {
	//----------
	Controller::Controller() {
		this->maximised = false;
		this->chromeVisible = true;
		this->mouseOwner = nullptr;
		this->lastClickOwner = nullptr;
		this->lastMouseClick = pair<long long, ofMouseEventArgs>(std::numeric_limits<long long>::min(), ofMouseEventArgs());
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
        ofAddListener(ofEvents().keyReleased, this, &Controller::keyReleased);    

		ofAddListener(ofEvents().fileDragEvent, this, &Controller::filesDragged);
		ofAddListener(ofEvents().windowResized, this, &Controller::windowResized);

		ofAddListener(ofEvents().exit, this, &Controller::exit, 0);

		ofxAssets::Register::X().addAddon("ofxCvGui");
		
		rootGroup->setBounds(ofGetCurrentViewport());
		this->rootGroup = rootGroup;
		this->currentPanel = PanelPtr();
		this->currentPanelBounds = ofGetCurrentViewport();

		//cache fonts
		ofxAssets::font("ofxCvGui::swisop3", 12);
		ofxAssets::font("ofxCvGui::swisop3", 14);
		ofxAssets::font("ofxCvGui::swisop3", 18);
		ofxAssets::font("ofxCvGui::swisop3", 24);
	}
	
	//----------
	void Controller::add(PanelPtr panel) {
		if (!this->rootGroup)
			return;
		this->rootGroup->add(panel);
	}

	//----------
	void Controller::remove(PanelPtr panel) {
		if (!this->rootGroup)
			return;
		this->rootGroup->remove(panel);
	}

	//----------
	void Controller::clear() {
		if (!this->rootGroup)
			return;
		this->rootGroup->clear();
	}
	
	//----------
	void Controller::toggleFullscreen() {
		ofToggleFullscreen();
	}

	//----------
	void Controller::toggleMaximised() {
		if (!this->maximised) {
			//maximise current panel
			auto currentPanel = this->currentPanel.lock();
			if (currentPanel) {
				this->setMaximised(currentPanel);
				currentPanel->setBounds(ofGetCurrentViewport());
			}
		} else {
			//clear maximise
			this->clearMaximised();
		}
	}
	
	//----------
	void Controller::setMaximised(PanelPtr panel) {
		this->maximised = true;
		this->currentPanel = panel;
		this->currentPanelBounds = ofGetCurrentViewport();
		panel->setBounds(ofRectangle(0, 0, ofGetScreenWidth(), ofGetScreenHeight()));
	}

	//----------
	void Controller::clearMaximised() {
		this->maximised = false;
		rootGroup->setBounds(ofGetCurrentViewport());
		this->updateCurrentPanel();
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
	void Controller::setActiveDialog(PanelPtr panel) {
		if (panel) {
			auto bounds = ofGetCurrentViewport();

			//first get a cached draw for the background
			this->activeDialogBackground.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

			//setup the active Dialog
			this->activeDialog = panel;
			
			//setup the size of the Dialog
			ofResizeEventArgs resizeArgs = {
				ofGetViewportWidth(),
				ofGetViewportHeight()
			};
			this->windowResized(resizeArgs);
		}
		else {
			this->closeActiveDialog();
		}
	}

	//----------
	void Controller::closeActiveDialog() {
		if (this->activeDialog) {
			this->onDialogClose.notifyListeners(this->activeDialog);
			this->activeDialog.reset();

			//setup the size of the root group
			ofResizeEventArgs resizeArgs = {
				ofGetViewportWidth(),
				ofGetViewportHeight()
			};
			this->windowResized(resizeArgs);
		}
	}

	//----------
	bool Controller::isDialogOpen() {
		return (this->activeDialog.get());
	}

	//----------
	void Controller::update(ofEventArgs& args) {
		if (!this->rootGroup) {
			return;
		}
		InspectController::X().update();

		if (this->activeDialog) {
			this->activeDialog->update();
		}
		else if (this->maximised) {
			this->currentPanel.lock()->update();
		}
		else {
			rootGroup->update();
		}
	}

	//----------
	void Controller::draw(ofEventArgs& args) {
        
//        ofEnableDepthTest();
      
        
		if (!this->rootGroup) {
			return;
		}

		DrawArguments rootDrawArguments;
		rootDrawArguments.chromeEnabled = this->chromeVisible;
		rootDrawArguments.naturalBounds = ofGetCurrentViewport();
		rootDrawArguments.globalTransform = glm::mat4();
		rootDrawArguments.globalScale = 1.0f;
		rootDrawArguments.localBounds = ofRectangle(0, 0, rootDrawArguments.naturalBounds.getWidth(), rootDrawArguments.naturalBounds.getHeight());
		rootDrawArguments.globalBounds = rootDrawArguments.naturalBounds;

		auto currentPanel = this->currentPanel.lock();

		if (this->activeDialog) {
			this->activeDialogBackground.draw(rootDrawArguments.naturalBounds);
			ofPushStyle();
			{
				//draw light box background
				ofEnableAlphaBlending();
				ofSetColor(0, 200);
				ofDrawRectangle(rootDrawArguments.naturalBounds);

				//shadow for dialog
				ofFill();
				ofSetColor(0, 100);
				ofPushMatrix();
				{
					ofTranslate(5, 5);
					ofDrawRectangle(this->activeDialog->getBounds());
				}
				ofPopMatrix();

				//background for dialog
				ofSetColor(80);
				ofDrawRectangle(this->activeDialog->getBounds());
			}
			ofPopStyle();
			this->activeDialog->draw(rootDrawArguments);
		}
		else {
			if (this->maximised) {
				currentPanel->draw(rootDrawArguments);
			}
			else {
				//highlight panel
				if (currentPanel) {
					ofPushStyle();
					ofEnableAlphaBlending();
					ofSetColor(40, 40, 40, 100);
					ofDrawRectangle(this->currentPanelBounds);
					ofPopStyle();
				}

				this->rootGroup->draw(rootDrawArguments);
			}
		}

		for (const auto & delayedDrawCommand : this->delayedDrawCommands) {
			delayedDrawCommand();
		}
		this->delayedDrawCommands.clear();
        
//         ofDisableDepthTest();
	}


	//----------
	void Controller::exit(ofEventArgs & args) {
		this->rootGroup.reset();

		ofRemoveListener(ofEvents().update, this, &Controller::update);
		ofRemoveListener(ofEvents().draw, this, &Controller::draw);
		ofRemoveListener(ofEvents().mouseMoved, this, &Controller::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &Controller::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &Controller::mouseReleased);
		ofRemoveListener(ofEvents().mouseDragged, this, &Controller::mouseDragged);
		ofRemoveListener(ofEvents().keyPressed, this, &Controller::keyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &Controller::keyReleased);
		ofRemoveListener(ofEvents().fileDragEvent, this, &Controller::filesDragged);
		ofRemoveListener(ofEvents().windowResized, this, &Controller::windowResized);
	}

	//----------
	PanelGroupPtr Controller::getRootGroup() const {
		return this->rootGroup;
	}

	//----------
	void Controller::setRootGroup(PanelGroupPtr rootGroup) {
		this->rootGroup = rootGroup;
		this->rootGroup->arrange();
	}

	//----------
	PanelPtr Controller::getPanelUnderCursor(const glm::vec2 & position) {
		if (this->maximised) {
			return currentPanel.lock();
		} else {
			ofRectangle panelBounds = this->rootGroup->getBounds();
			return this->findPanelUnderCursor(panelBounds, position);
		}
	}

	//----------
	void Controller::drawDelayed(function<void()> && drawFunction) {
		this->delayedDrawCommands.emplace_back(drawFunction);
	}

	//----------
	void Controller::mouseMoved(ofMouseEventArgs & args) {
		if (!this->rootGroup) {
			return;
		}
		auto currentPanel = this->currentPanel.lock();
		MouseArguments action(MouseArguments(args, MouseArguments::Moved, rootGroup->getBounds(), currentPanel, this->mouseOwner));

		this->mouseAction(action);

		this->updateCurrentPanel();
	}
	
	//----------
	void Controller::mousePressed(ofMouseEventArgs & args) {
		if (!this->rootGroup) {
			return;
		}

		auto thisMouseClick = pair<long long, ofMouseEventArgs>(ofGetElapsedTimeMillis(), args);

		bool isDoubleClick = (thisMouseClick.first - this->lastMouseClick.first) < OFXCVGUI_DOUBLECLICK_TIME_THRESHOLD_MS;

		auto distanceSinceLastClick = glm::distance(
			(glm::vec2) thisMouseClick.second,
			(glm::vec2) this->lastMouseClick.second
		);
		isDoubleClick &= distanceSinceLastClick < OFXCVGUI_DOUBLECLICK_SPACE_THRESHOLD_PX;

		if (isDoubleClick) {
			this->mouseOwner = this->lastClickOwner;
		}

		auto currentPanel = this->currentPanel.lock();
		auto action = MouseArguments(args, isDoubleClick ? MouseArguments::Action::DoubleClick : MouseArguments::Action::Pressed, rootGroup->getBounds(), currentPanel, this->mouseOwner);

		if (this->activeDialog && !this->activeDialog->getBounds().inside(action.local)) {
			this->closeActiveDialog();
		}
		else {
			this->mouseAction(action);
		}

        this->mouseCached = action.global;
		this->mouseOwner = action.getOwner();
		this->lastMouseClick = thisMouseClick;
	}
	
	//----------
	void Controller::mouseReleased(ofMouseEventArgs & args) {
		if (!this->rootGroup) {
			return;
		}

		auto currentPanel = this->currentPanel.lock();
		MouseArguments action(args, MouseArguments::Released, rootGroup->getBounds(), currentPanel, this->mouseOwner);

		this->mouseAction(action);

		this->lastClickOwner = this->mouseOwner;
		this->mouseOwner = nullptr;
	}
	
	//----------
	void Controller::mouseDragged(ofMouseEventArgs & args) {
		if (!this->rootGroup) {
			return;
		}

		auto currentPanel = this->currentPanel.lock();
		MouseArguments action(args, MouseArguments::Dragged, rootGroup->getBounds(), currentPanel, this->mouseOwner, mouseCached);
		this->mouseAction(action);

		this->mouseCached = action.global;
	}

	//----------
	void Controller::mouseAction(MouseArguments & action) {
		if (this->activeDialog) {
			this->activeDialog->mouseAction(action);
		}
		else {
			auto currentPanel = this->currentPanel.lock();
			if (this->maximised) {
				currentPanel->mouseAction(action);
			}
			else {
				rootGroup->mouseAction(action);
			}
		}
	}
	
	//----------
    void Controller::keyReleased(ofKeyEventArgs & args) {
        if (!this->rootGroup) {
            return;
        }
        auto currentPanel = this->currentPanel.lock();
        KeyboardArguments action(args, KeyboardArguments::Released, currentPanel);
        if (this->activeDialog) {
//            if (args.key == OF_KEY_ESC) {
//                this->closeActiveDialog();
//            }
//            else {
                this->activeDialog->keyboardAction(action);
//            }
        }
        else {
            if (this->maximised) {
                //if something is maximised, only it get the key press
                currentPanel->keyboardAction(action);
            }
            else {
                //otherwise everything visible gets the key press
                rootGroup->keyboardAction(action);
            }
        }
    }
	void Controller::keyPressed(ofKeyEventArgs & args) {
		if (!this->rootGroup) {
			return;
		}

		if (!this->activeDialog) {
			if (args.key == 'f')
				this->toggleFullscreen();
			if (args.key == 'm')
				this->toggleMaximised();
		}

		auto currentPanel = this->currentPanel.lock();
		KeyboardArguments action(args, KeyboardArguments::Pressed, currentPanel);
		if (this->activeDialog) {
			if (args.key == OF_KEY_ESC) {
				this->closeActiveDialog();
			}
			else {
				this->activeDialog->keyboardAction(action);
			}
		}
		else {
			if (this->maximised) {
				//if something is maximised, only it get the key press
				currentPanel->keyboardAction(action);
			}
			else {
				//otherwise everything visible gets the key press
				rootGroup->keyboardAction(action);
			}
		}
	}

	//----------
	void Controller::filesDragged(ofDragInfo & args) {
		if (!this->rootGroup) {
			return;
		}

		auto rootBounds = this->rootGroup->getBounds();
		auto panel = this->findPanelUnderCursor(rootBounds);
		if (panel != PanelPtr()) {
			auto panelBounds = panel->getBounds();
			auto panelTopLeft = panelBounds.getTopLeft();
			auto newArgs = FilesDraggedArguments((glm::vec2) args.position - panelTopLeft, (glm::vec2) args.position, args.files);
			panel->onFilesDragged(newArgs);
		}
	}

	//----------
	void Controller::windowResized(ofResizeEventArgs & args) {
		if (!this->rootGroup) {
			return;
		}

		const auto viewportBounds = ofRectangle(0, 0, args.width, args.height);
		if (this->activeDialog) {
			const auto padding = 80.0f;
			ofRectangle bounds = viewportBounds;
			bounds.x += padding;
			bounds.y += padding;
			bounds.width -= padding * 2.0f;
			bounds.height -= padding * 2.0f;

			//if bounds are too small, use all of it
			if (bounds.width < 200 || bounds.height < 200) {
				bounds = viewportBounds;
			}
			this->activeDialog->setBounds(bounds);
		}
		else {
			auto currentPanel = this->currentPanel.lock();
			if (this->maximised) {
				currentPanel->setBounds(viewportBounds);
			}
			else {
				this->rootGroup->setBounds(viewportBounds);
			}
		}
	}

	//----------
	bool Controller::checkInitialised() {
		if (this->rootGroup)
			return true;
		else {
			ofLogError("ofxCvGui") << "cannot perform this action as gui is not initialised";
			return false;
		}
	}

	//----------
	PanelPtr Controller::findPanelUnderCursor(ofRectangle & panelBounds, const glm::vec2 & position) {
		if (!this->rootGroup) {
			return PanelPtr();
		}

		if (this->activeDialog) {
			return activeDialog;
		}
		else if (this->maximised) {
			return this->currentPanel.lock();
		}
		else {
			return rootGroup->findScreen(position, panelBounds);
		}
	}

	//----------
	void Controller::updateCurrentPanel() {
		if (!this->maximised) {
			auto currentPanelBounds = this->rootGroup->getBounds();
			this->currentPanel = this->findPanelUnderCursor(currentPanelBounds);
			this->currentPanelBounds = currentPanelBounds;
		}
	}

	//----------
	ofxCvGui::PanelPtr Controller::getActiveDialog() {
		return this->activeDialog;
	}

	//----------
	void openDialog(PanelPtr panel) {
		Controller::X().setActiveDialog(panel);
	}

	//----------
	void closeDialog(Panels::Base * panel) {
		if (Controller::X().getActiveDialog().get() == panel) {
			Controller::X().closeActiveDialog();
		}
	}

	//----------
	void closeDialog() {
		Controller::X().closeActiveDialog();
	}

	//----------
	bool isDialogOpen() {
		return Controller::X().isDialogOpen();
	}
}
