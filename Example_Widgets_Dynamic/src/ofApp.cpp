#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	//make things pretty
	ofEnableSmoothing();
	
	//set a starting circle resolution
	ofSetCircleResolution(32);
	this->circleResolution = 32;

	//setup the gui
	this->gui.init();
	
	//build the drawPanel
	auto drawPanel = this->gui.addBlank();
	drawPanel->onDraw += [this] (ofxCvGui::DrawArguments &) {
		for(auto & circle : this->circles) {
			ofPushStyle();
			if (circle->fill) {
				ofFill();
			} else {
				ofNoFill();
			}
			ofSetColor(circle->luminance);
			
			ofPushMatrix();
			ofTranslate(circle->x, circle->y);
			ofRotate(circle->rotation);
			
			ofCircle(0, 0, circle->radius);
			
			ofPopMatrix();
			
			ofPopStyle();
		}
	};
	
	//add the scroll panel which we'll need to edit later on
	this->scrollPanel = this->gui.addScroll();
	
	//add one circle to the set to begin with
	this->addCircle();
	
	//perform the initial rebuild of widgets
	this->rebuildWidgets();
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetCircleResolution(this->circleResolution);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
}

//--------------------------------------------------------------
void ofApp::addCircle() {
	auto circle = make_shared<Circle>();
	
	// Initialise the parameters for each circle
	circle->name = "Circle #" + ofToString(this->circles.size());
	circle->x.set("X", ofRandomuf() * 512.0f, 0.0f, 512.0f);
	circle->y.set("Y", ofRandomuf() * 512.0f, 0.0f, 512.0f);
	circle->radius.set("Radius", ofRandomuf() * 100.0f, 0.0f, 100.0f);
	circle->luminance.set("Luminance", floor(ofRandomuf() * 255.0f), 0.0f, 255.0f);
	circle->fill.set("Fill", false);
	circle->rotation.set("Rotation", 0.0f, -180.0f, 180.0f);
	
	this->circles.push_back(circle);
}

//--------------------------------------------------------------
void ofApp::rebuildWidgets() {
	this->scrollPanel->clear();
	
	// ------------------------
	// ADD SOME GENERAL WIDGETS
	// ------------------------
	//
	
	// FRAME RATE WIDGET
	// -----------------
	// This widget accepts a lambda function which is polled every time the widget draws.
	// make_shared<Widgets::LiveValueHistory>(Caption, lambda function which returns a float)
	auto framerateWidget = make_shared<Widgets::LiveValueHistory>("Frame rate [hz]", [] () {
		return ofGetFrameRate();
	});
	this->scrollPanel->add(framerateWidget);
	
	
	// ADD BUTTON WIDGET
	// -----------------
	// This widget accepts a lambda function which is called when the mouse is pressed and
	// then released inside the active area of the button.
	// make_shared<Widgets::Button>(Caption, lambda function which is called on button hit)
	this->scrollPanel->add(make_shared<Widgets::Button>("Add circle", [this]() {
		this->addCircle();
		this->rebuildWidgets();
	}));
	
	
	// CIRCLE RESOLUTION SELECTION WIDGET
	// ----------------------------------
	// This widget needs to be filled with options.
	// You can find out which option is selected with getSelection() or getSelectionIndex().
	// Also you can listen for callbacks with onSelectionChange
	auto circleResolutionSelector = make_shared<Widgets::MultipleChoice>("Circle resolution");
	circleResolutionSelector->addOption("3");
	circleResolutionSelector->addOption("6");
	circleResolutionSelector->addOption("32");
	// Here we pass the the widget into a function which will call getSelection on the widget.
	// Since the shared_ptr will be inside the function, and the function is owned by the widget,
	//  then we will end up with a circular reference (shared_ptr owns widget owns function owns shared_ptr).
	// To avoid this issue we must use a weak_ptr (best google weak_ptr!)
	weak_ptr<Widgets::MultipleChoice> circleResolutionSelectorWeak = circleResolutionSelector;
	circleResolutionSelector->onSelectionChange += [this, circleResolutionSelectorWeak] (int selectionIndex) {
		auto circleResolutionSelector = circleResolutionSelectorWeak.lock();
		if (circleResolutionSelector) {
			this->circleResolution = ofToInt(circleResolutionSelector->getSelection());
		}
	};
	// Set the selection to reflect the current active value
	circleResolutionSelector->setSelection(ofToString(this->circleResolution));
	this->scrollPanel->add(circleResolutionSelector);
	
	// SPACER WIDGET
	// -------------
	// This widget just adds 3 dots
	this->scrollPanel->add(make_shared<Widgets::Spacer>());
	
	//
	// ------------------------
	
	
	// -------------------------------
	// ADD THE WIDGETS FOR EACH CIRCLE
	// -------------------------------
	//
	
	for(auto circle : this->circles) {
		scrollPanel->add(ElementPtr(new Widgets::Title(circle->name, Widgets::Title::Level::H2)));
		
		auto insideIndicator = make_shared<Widgets::Indicator>("Mouse inside", [this, circle]() {
			const auto mouse = ofVec2f(ofGetMouseX(), ofGetMouseY());
			const auto circleCenter = ofVec2f(circle->x, circle->y);
			return (mouse - circleCenter).length() < circle->radius ? Widgets::Indicator::Status::Good : Widgets::Indicator::Status::Clear;
		});
		scrollPanel->add(insideIndicator);
		
		//add a button which can rename this circle
		auto renameButton = make_shared<Widgets::Button>("Rename", [=] {
			auto result = ofSystemTextBoxDialog("Rename " + circle->name);
			if(result != "") {
				circle->name = result;
				this->rebuildWidgets();
			}
		});
		scrollPanel->add(renameButton);
		
		//add a button which will delete this circle
		auto deleteButton = make_shared<Widgets::Button>("Delete", [=] {
			//find the circle in the vector
			auto it = this->circles.begin();
			for (; it != this->circles.end(); it++) {
				if(*it == circle) {
					break; // break out of the loop if this is the right one
				}
			}
			
			if(it != this->circles.end()) {
				//then we found the circle in the vector
				this->circles.erase(it);
				this->rebuildWidgets();
			}
		});
		scrollPanel->add(deleteButton);
		
		scrollPanel->add(make_shared<Widgets::Slider>(circle->x));
		scrollPanel->add(make_shared<Widgets::Slider>(circle->y));
		scrollPanel->add(make_shared<Widgets::Slider>(circle->radius));
		scrollPanel->add(make_shared<Widgets::Toggle>(circle->fill));
		
		auto rotationSlider = make_shared<Widgets::Slider>(circle->rotation);
		rotationSlider->addStepValidator(10.0f);
		scrollPanel->add(rotationSlider);
		
		//special slider with a validator to check for integer values
		auto luminanceSlider = make_shared<Widgets::Slider>(circle->luminance);
		luminanceSlider->addIntValidator();
		scrollPanel->add(luminanceSlider);
	
		scrollPanel->add(make_shared<Widgets::Spacer>());
	}
	
	//
	// -------------------------------
	
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
