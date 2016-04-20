#pragma once
#include "ofMain.h"

#include "ofxCvGui/Panels/Groups/Base.h"
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Types.h"

#define OFXCVGUI_DOUBLECLICK_SPACE_THRESHOLD_PX 3
#define OFXCVGUI_DOUBLECLICK_TIME_THRESHOLD_MS 250


namespace ofxCvGui {
	class Controller : public ofxSingleton::Singleton<Controller> {
	public:
		Controller();
		void init(PanelGroupPtr rootGroup);

		void add(PanelPtr panel);
		void drop(PanelPtr panel);
		void clear();

		void toggleFullscreen();
		void toggleMaximised();

		void setMaximised(PanelPtr panel);
		void clearMaximised();

		void showChrome();
		void hideChrome();

		void setActiveDialogue(PanelPtr);
		PanelPtr getActiveDialogue();
		void clearActiveDialogue();
		bool isDialogueOpen();

		PanelGroupPtr getRootGroup() const;
		void setRootGroup(PanelGroupPtr);

		PanelPtr getPanelUnderCursor(const ofVec2f & position = ofVec2f(ofGetMouseX(), ofGetMouseY()));

	protected:
		////
		//actions
		////
		//
		void update(ofEventArgs & args);
		void draw(ofEventArgs & args);
		void mouseMoved(ofMouseEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		void mouseReleased(ofMouseEventArgs & args);
		void mouseDragged(ofMouseEventArgs & args);
		void keyPressed(ofKeyEventArgs & args);
		void filesDragged(ofDragInfo & args);
		void windowResized(ofResizeEventArgs & args);
		//
		////

		void mouseAction(MouseArguments &);

		bool checkInitialised();
		PanelPtr findPanelUnderCursor(ofRectangle & panelBounds, const ofVec2f & position = ofVec2f(ofGetMouseX(), ofGetMouseY())); ///<input root bounds to get the found panel's bounds. should split  to getGlobalBoundsOfPanel(PanelPtr)
		void updateCurrentPanel();
		bool initialised;

		PanelGroupPtr rootGroup;
		weak_ptr<Panels::Base> currentPanel;

		PanelPtr activeDialogue;
		ofFbo activeDialogueBackground;

		ofRectangle currentPanelBounds;
		bool maximised;
        ofVec2f mouseCached;
		void * mouseOwner;
		void * lastClickOwner;
		pair<long long, ofMouseEventArgs> lastMouseClick; ///<timestamp in millis
		bool chromeVisible;

		float cachedWidth, cachedHeight;
	};

	void openDialogue(PanelPtr);
	void closeDialogue(Panels::Base * );
	void closeDialogue();
	bool isDialogueOpen();
}