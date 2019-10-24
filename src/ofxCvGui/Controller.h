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
		void remove(PanelPtr panel);
		void clear();

		void toggleFullscreen();
		void toggleMaximised();

		void setMaximised(PanelPtr panel);
		void clearMaximised();

		void showChrome();
		void hideChrome();

		void setActiveDialog(PanelPtr);
		PanelPtr getActiveDialog();
		void closeActiveDialog();
		bool isDialogOpen();

		PanelGroupPtr getRootGroup() const;
		void setRootGroup(PanelGroupPtr);

		PanelPtr getPanelUnderCursor(const glm::vec2 & position = glm::vec2(ofGetMouseX(), ofGetMouseY()));

		ofxLiquidEvent<PanelPtr> onDialogClose;
		void drawDelayed(function<void()> && drawFunction);
	protected:
		////
		//actions
		////
		//
		void update(ofEventArgs & args);
		void draw(ofEventArgs & args);
		void exit(ofEventArgs & args);
		void mouseMoved(ofMouseEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		void mouseReleased(ofMouseEventArgs & args);
		void mouseDragged(ofMouseEventArgs & args);
		void keyPressed(ofKeyEventArgs & args);
        void keyReleased(ofKeyEventArgs & args);
		void filesDragged(ofDragInfo & args);
		void windowResized(ofResizeEventArgs & args);
		//
		////

		void mouseAction(MouseArguments &);

		bool checkInitialised();
		PanelPtr findPanelUnderCursor(ofRectangle & panelBounds, const glm::vec2 & position = glm::vec2(ofGetMouseX(), ofGetMouseY())); ///<input root bounds to get the found panel's bounds. should split  to getGlobalBoundsOfPanel(PanelPtr)
		void updateCurrentPanel();

		PanelGroupPtr rootGroup;
		weak_ptr<Panels::Base> currentPanel;

		PanelPtr activeDialog;
		ofImage activeDialogBackground;

		ofRectangle currentPanelBounds;
		bool maximised;
		glm::vec2 mouseCached;
		void * mouseOwner;
		void * lastClickOwner;
		pair<long long, ofMouseEventArgs> lastMouseClick; ///<timestamp in millis
		bool chromeVisible;

		float cachedWidth, cachedHeight;

		vector<function<void()>> delayedDrawCommands;
	};

	void openDialog(PanelPtr);
	void closeDialog(Panels::Base * );
	void closeDialog();
	bool isDialogOpen();
}
