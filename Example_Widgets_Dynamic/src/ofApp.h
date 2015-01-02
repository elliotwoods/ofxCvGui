#pragma once

#include "ofMain.h"
#include "ofxCvGui.h"

class ofApp : public ofBaseApp{

	struct Circle {
		string name;
		ofParameter<float> radius;
		ofParameter<float> x;
		ofParameter<float> y;
		ofParameter<float> luminance;
		ofParameter<bool> fill;
		ofParameter<float> rotation;
	};
	
public:
	void setup();
	void update();
	void draw();

	void addCircle();
	void rebuildWidgets();

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
	shared_ptr<ofxCvGui::Panels::Scroll> scrollPanel;
	
	vector<shared_ptr<Circle>> circles;
	
	int circleResolution;
};
