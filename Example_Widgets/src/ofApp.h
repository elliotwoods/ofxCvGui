#pragma once

#include "ofMain.h"
#include "ofxCvGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxCvGui::Builder gui;

		enum Color {
			White,
			Orange,
			Purple
		};

		ofParameter<string> name[5];
		ofParameter<float> radius[5];
		ofParameter<float> x[5];
		ofParameter<float> y[5];
		ofParameter<Color> color[5];
		ofParameter<float> luminance[5];
		ofParameter<bool> fill[5];
};
