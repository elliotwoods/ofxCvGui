#pragma once
#include "ofMain.h"

#include "ofxCvGui/Panels/Groups/Base.h"
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Types.h"
#include "ofxCvGui/Assets.h"

namespace ofxCvGui {
	class Controller {
	public:
		Controller();
		void init(PanelGroupPtr rootGroup);
		void add(PanelPtr& panel);
	protected:
		////
		//actions
		////
		//
		void update(ofEventArgs &args);
		void draw(ofEventArgs &args);
		void mouseMoved(ofMouseEventArgs &args);
		void mousePressed(ofMouseEventArgs &args);
		void mouseReleased(ofMouseEventArgs &args);
		void mouseDragged(ofMouseEventArgs &args);
		void keyPressed(ofKeyEventArgs &args);
		void windowResized(ofResizeEventArgs &args);
		//
		////

		bool checkInitialised();
		bool initialised;
		PanelGroupPtr rootGroup;
	};
}