#include "ofApp.h"

using namespace ofxCvGui;

class MyElement : public ofxCvGui::Element {
public:
	MyElement(){
		this->onDraw += [this](ofxCvGui::DrawArguments & args) {
			//black background
			ofPushStyle();
			ofFill();
			ofSetColor(0);
			ofRect(args.localBounds);
			ofPopStyle();

			ofPushStyle();
			if (this->getMouseState() == ofxCvGui::Element::LocalMouseState::Waiting) {
				ofNoFill();
				ofRect(args.localBounds);
			}
			else if (this->getMouseState() == ofxCvGui::Element::LocalMouseState::Down) {
				ofFill();
				ofSetColor(100);
				ofRect(args.localBounds);
				ofFill();
			}
			else if (this->getMouseState() == ofxCvGui::Element::LocalMouseState::Dragging) {
				ofFill();
				ofSetColor(200, 0, 0);
				ofRect(args.localBounds);
				ofFill();
			}
			ofPopStyle();

			ofDrawBitmapString(this->getCaption(), 5, 15);
		};

		this->onMouse += [this](ofxCvGui::MouseArguments & args) {
			//this function does a few things:
			// * check if the mouse press if local
			// * if so, take ownership of the mouse press
			// * also the Element will change the this->getMouseState() == LocalMouseState::Down (note : this happens after this onMouse function returns)
			args.takeMousePress(this);
		};
	}

	void addChild(ofxCvGui::ElementPtr child) {
		this->child = child;
		child->addListenersToParent(this);
	}

	ElementPtr child;
};
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetCircleResolution(50);
	ofEnableSmoothing();

	this->gui.init();

	//make a panel which we can add elements to
	auto elementsPanel = make_shared<ofxCvGui::Panels::ElementHost>();

	//add that panel to the gui
	this->gui.add(elementsPanel);





	//make the basic elements
	auto element1 = make_shared<MyElement>();
	auto element2 = make_shared<MyElement>();
	
	//set captions
	element1->setCaption("1");
	element2->setCaption("2");

	//set the bounds of the elements
	element1->setBounds(ofRectangle(100, 100, 200, 200));
	element2->setBounds(ofRectangle(200, 200, 200, 200));

	//add them to the elements group panel
	elementsPanel->getElementGroup()->add(element1);
	elementsPanel->getElementGroup()->add(element2);





	//make a nested element
	auto element1_1 = make_shared<MyElement>();

	//set caption
	element1_1->setCaption("1_1");

	//set the local bounds of the nested element
	element1_1->setBounds(ofRectangle(50, 50, 100, 100));

	//add the nested element to our existing element
	element1->addChild(element1_1);
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
