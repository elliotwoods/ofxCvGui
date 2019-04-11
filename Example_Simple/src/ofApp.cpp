#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	this->camera.initGrabber(1280, 720);

	// Must be called before using the gui
	this->gui.init();

	// The gui accepts a range of object types, in this case we rely on the fact that ofVideoGrabber inherits from ofBaseDraws
	this->gui.add(this->camera, "Camera");

	// We have a special panel type for ofImage which can do things like save/load to file, so the special type is used instead of the one for ofBaseDraws
	this->gui.add(this->inverse, "Inverse");
}

//--------------------------------------------------------------
void ofApp::update(){
	this->camera.update();

	//copy the pixels across
	this->inverse.getPixels() = this->camera.getPixels();

	//invert
	for (auto & pixel : this->inverse.getPixels()) {
		pixel = 255 - pixel;
	}

	//update the texture inside the ofImage
	this->inverse.update();

	// NB : It would be slightly slightly quicker to invert whilst copying, but the code would be much longer, and for this case it doesn't matter :)
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
