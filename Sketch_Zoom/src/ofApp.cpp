#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetCircleResolution(50);
	ofEnableSmoothing();
	ofSetBackgroundColor(10);

	this->gui.init();

	auto elementPanel = make_shared<Panels::ElementCanvas>();
	this->gui.add(elementPanel);

	ofSeedRandom(0);

	auto makeTestElement = [](int i) {
		auto element = make_shared<Element>();
		auto testNum = (int)ofRandom(100.0f);

		auto elementWeak = weak_ptr<Element>(element);
		element->onDraw += [testNum, elementWeak](DrawArguments & args) {
			auto element = elementWeak.lock();
			ofClear(255);

			ofPushStyle();
			if (element->isMouseDown() || element->isMouseDragging()) {
				ofSetColor(100);
			}
			else if (element->isMouseOver()) {
				ofSetColor(60);
			}
			else {
				ofSetColor(40);
			}
			ofRect(args.localBounds);
			ofPopStyle();

			Utils::drawText(ofToString(testNum), args.localBounds, false);
			cout << testNum << " ";
		};
		element->setScissor(true); //we want to test 
		element->onMouse += [elementWeak](MouseArguments & args) {
			auto element = elementWeak.lock();
			args.takeMousePress(element);

			if (args.isDragging(element)) {
				auto bounds = element->getBounds();
				bounds.x += args.movement.x;
				bounds.y += args.movement.y;
				element->setBounds(bounds);
			}
		};

		element->setBounds(ofRectangle((i % 4) * 200, (i / 4) * 100, 100, 50));
		return element;
	};

	for (int i = 0; i < 10; i++) {
		elementPanel->getCanvasElementGroup()->add(makeTestElement(i));
	}

	elementPanel->getCanvasElementGroup()->onDraw += [](ofxCvGui::DrawArguments & args) {
		ofPushStyle();
		ofSetLineWidth(1.0f);
		ofNoFill();
		ofRect(args.localBounds);
		ofPopStyle();
	};







	/*
	when do we need to zoom?
		- ElementCanvas
		- Scroll?
		- Text editor / HTML style pages (e.g. Inspector)

		should it be only on ElementCanvas?

		zoomed objects generally need to also support panning (i.e. ElementCanvas)

		what happens to ->getBounds()? this could cause many issues (e.g. if )

		let's implement for ElementCanvas only for now
		*/
}

//--------------------------------------------------------------
void ofApp::update(){
	cout << endl;
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
