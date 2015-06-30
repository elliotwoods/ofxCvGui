#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetCircleResolution(50);
	ofEnableSmoothing();
	ofSetLogLevel(OF_LOG_VERBOSE);

	this->gui.init();
	auto & assetRegister = ofxAssets::AssetRegister();

	auto drawPanel = this->gui.addBlank();
	auto scrollPanel = this->gui.addScroll();

	for(int i=0; i<5; i++) {
		this->x[i].set("X", ofRandomuf() * 512.0f, 0.0f, 512.0f);
		this->y[i].set("Y", ofRandomuf() * 512.0f, 0.0f, 512.0f);
		this->radius[i].set("Radius", ofRandomuf() * 100.0f, 0.0f, 100.0f);
		this->luminance[i].set("Luminance", floor(ofRandomuf() * 255.0f), 0.0f, 255.0f);
		this->fill[i].set("Fill", false);

		scrollPanel->add(ElementPtr(new Widgets::Slider(this->x[i])));
		scrollPanel->add(ElementPtr(new Widgets::Slider(this->y[i])));
		scrollPanel->add(ElementPtr(new Widgets::Slider(this->radius[i])));
		scrollPanel->add(ElementPtr(new Widgets::Toggle(this->fill[i])));

		//special slider with a validator to check for whole values
		auto luminanceSlider = new Widgets::Slider(this->luminance[i]);
		luminanceSlider->addIntValidator();
		scrollPanel->add(ElementPtr(luminanceSlider));

		scrollPanel->add(ElementPtr(new Widgets::Spacer()));
	}

	drawPanel->onDraw += [this] (ofxCvGui::DrawArguments &) {
		for(int i=0; i<5; i++) {
			ofPushStyle();
			this->fill[i].get() ? ofFill() : ofNoFill();
			ofSetColor(this->luminance[i]);
			ofCircle(this->x[i], this->y[i], this->radius[i]);
			ofPopStyle();
		}
	};
}

//--------------------------------------------------------------
void ofApp::update(){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
