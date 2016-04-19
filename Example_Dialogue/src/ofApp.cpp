#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	this->gui.init();

	//add a panel of circles
	this->gui.addBlank()->onDraw += [this](ofxCvGui::DrawArguments & args) {
		ofPushMatrix();
		ofPushStyle();
		{
			ofTranslate(args.localBounds.getCenter());
			for (float i = 0; i < TWO_PI; i += 0.2f) {
				ofColor color(200, 0, 0);
				color.setHueAngle(i * RAD_TO_DEG);
				ofSetColor(color);
				ofDrawCircle(sin(i) * 50.0f * i, cos(i) * 50.0f * i, 30);
			}
		}
		ofPopStyle();
		ofPopMatrix();
	};

	//add a panel of squares
	this->gui.addBlank()->onDraw += [this](ofxCvGui::DrawArguments & args) {
		ofPushMatrix();
		ofPushStyle();
		{
			ofTranslate(args.localBounds.getCenter());
			for (float i = 0; i < TWO_PI; i += 1.0f) {
				ofColor color(200, 100, 100);
				color.setHue(i * RAD_TO_DEG);
				ofSetColor(color);
				ofDrawRectangle(sin(i) * 100.0f, cos(i) * 100.0f, 30, 30);
			}
		}
		ofPopStyle();
		ofPopMatrix();
	};

	auto widgets = this->gui.addWidgets();
	widgets->addTitle("Dialogue test");
	widgets->addButton("Open dialogue", [this]() {

		//--
		//Build the dialogue
		//--
		//
		auto dialogue = make_shared<ofxCvGui::Panels::Widgets>();
		dialogue->addTitle("Dialogue");
		dialogue->addSpacer();
		dialogue->addFps();
		dialogue->addMemoryUsage();
		for (int i = 0; i < 30; i++) {
			dialogue->addIndicator("Indicator", [i]() {
				if (ofGetFrameNum() % 30 == i) {
					return ofxCvGui::Widgets::Indicator::Status::Good;
				}
				else {
					return ofxCvGui::Widgets::Indicator::Status::Clear;
				}
			});
		}
		dialogue->addButton("Close", []() {
			ofxCvGui::Controller::X().clearActiveDialogue();
		});
		//
		//--

		// Open the dialogue
		ofxCvGui::openDialogue(dialogue);
	});
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
