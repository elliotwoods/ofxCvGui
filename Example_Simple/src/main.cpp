#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings windowSettings;
	windowSettings.setGLVersion(4, 0);
	windowSettings.setSize(1920, 1080);
	windowSettings.depthBits = 32;
	auto window = ofCreateWindow(windowSettings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
