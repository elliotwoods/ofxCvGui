#pragma once
#include "ofMain.h"

#include "ofxCvGui/Panels/Groups/Base.h"
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Types.h"

#define OFXCVGUI_DOUBLECLICK_SPACE_THRESHOLD_PX 3
#define OFXCVGUI_DOUBLECLICK_TIME_THRESHOLD_MS 500


namespace ofxCvGui {
	class Controller {
	public:
		Controller();
		void init(PanelGroupPtr rootGroup);
		void add(PanelPtr panel);
		void drop(PanelPtr panel);
		void clear();
		void toggleMaximised();
		void toggleFullscreen();
		void setFullscreen(PanelPtr panel);
		void clearFullscreen();
		void showChrome();
		void hideChrome();
		PanelGroupPtr getRootGroup();
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
		//
		////

		bool checkInitialised();
		PanelPtr findPanelUnderCursor(ofRectangle & panelBounds, const ofVec2f & position = ofVec2f(ofGetMouseX(), ofGetMouseY())); ///<input root bounds to get the found panel's bounds. should split  to getGlobalBoundsOfPanel(PanelPtr)
		void updateCurrentPanel();
		bool initialised;
		PanelGroupPtr rootGroup;
		PanelPtr currentPanel;
		ofRectangle currentPanelBounds;
		bool maximised;
		bool fullscreen;
        ofVec2f mouseCached;
		void * mouseOwner;
		pair<long long, ofMouseEventArgs> lastMouseClick; ///<timestamp in millis
		bool chromeVisible;

		float cachedWidth, cachedHeight;
		int lastRebuildRequiredFrame;
	};
}