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
	widgets->addButton("Open dialog", [this]() {

		//--
		//Build the dialog
		//--
		//
		auto dialog = make_shared<ofxCvGui::Panels::Widgets>();
		dialog->addTitle("Dialog");
		dialog->addSpacer();
		dialog->addFps();
		dialog->addMemoryUsage();
		for (int i = 0; i < 30; i++) {
			dialog->addIndicator("Indicator", [i]() {
				if (ofGetFrameNum() % 30 == i) {
					return ofxCvGui::Widgets::Indicator::Status::Good;
				}
				else {
					return ofxCvGui::Widgets::Indicator::Status::Clear;
				}
			});
		}
		dialog->addButton("Close", []() {
			ofxCvGui::closeDialog();
		});
		//
		//--

		// Open the Dialog
		ofxCvGui::openDialog(dialog);
	});

	//this callback happens when the dialog closes
	ofxCvGui::Controller::X().onDialogClose += [](ofxCvGui::PanelPtr &) {
		ofSystemAlertDialog("Dialog closed");
	};
	//Note
	// 1. If you add a listener here, you probably want to remove it later on
	//	(e.g. if you disappear but the gui is still around and other dialogs
	//	might open).
	// 2. You might want to check that the dialog being closed is your dialog
	//	if you're going to perform any action as a result of it being closed.
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
