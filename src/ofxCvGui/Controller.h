#pragma once
#include "ofMain.h"

#include "ofxCvGui/Element.h"
#include "ofxCvGui/Types.h"
#include "ofxCvGui/Groups/PanelGroup.h"

namespace ofxCvGui {
	class Controller {
	public:
		Controller();
		void init();
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
		ElementPtr baseElement;
	};
}