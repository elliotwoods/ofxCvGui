#include "ofApp.h"

#include "ofxAssets.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetCircleResolution(50);
	ofEnableSmoothing();
	ofSetLogLevel(OF_LOG_VERBOSE);

	this->gui.init();
	auto & assetRegister = ofxAssets::AssetRegister();

	auto drawPanel = this->gui.addBlank();
	auto widgetsPanel = this->gui.addWidgets();

	for(int i=0; i<5; i++) {
		this->name[i].set("Name", ofToString(i));
		this->x[i].set("X", ofRandomuf() * 512.0f, 0.0f, 512.0f);
		this->y[i].set("Y", ofRandomuf() * 512.0f, 0.0f, 512.0f);
		this->radius[i].set("Radius", ofRandomuf() * 100.0f, 0.0f, 100.0f);
		this->luminance[i].set("Luminance", floor(ofRandomuf() * 255.0f), 0.0f, 255.0f);
		this->fill[i].set("Fill", false);

		// Add an EditableValue
		widgetsPanel->addEditableValue(this->name[i]);

		// Adding a slider : Syntax 1 - "Helper function"
		// Here we use the Widgets Panel's helper function 'addSlider' to make a slider for us
		widgetsPanel->addSlider(this->x[i]);

		// Adding a slider : Syntax 2 - "new"
		// Here we create the new widget ourselves as a pointer, and pass it to the Widgets Panel
		widgetsPanel->add(new Widgets::Slider(this->y[i]));

		// Adding a slider : Syntax 3 - "make_shared"
		// Here we create the new widget as a shared_ptr, and pass it to the Widgets Panel
		widgetsPanel->add(make_shared<Widgets::Slider>(this->radius[i]));

		// Adding a multiple choice, and entangling it with an ofParameter
		auto colorChooser = widgetsPanel->addMultipleChoice("Color", { "White", "Orange", "Purple" });
		colorChooser->entangle(this->color[i]);

		// Adding a slider and modifying its properties
		auto luminanceSlider = widgetsPanel->addSlider(this->luminance[i]);
		luminanceSlider->addIntValidator(); // this slider will now 'snap' to integer values

		// Add a toggle 
		widgetsPanel->addToggle(this->fill[i]);
		

		widgetsPanel->addSpacer();
	}

	drawPanel->onDraw += [this] (ofxCvGui::DrawArguments &) {
		for(int i=0; i<5; i++) {
			ofPushStyle();
			{
				//apply fill
				this->fill[i].get() ? ofFill() : ofNoFill();

				//apply color (enum)
				ofColor color;
				switch (this->color[i].get()) {
				case White:
					color = ofColor::white;
					break;
				case Orange:
					color = ofColor::orange;
					break;
				case Purple:
					color = ofColor::purple;
					break;
				default:
					break;
				}
				color.setBrightness(this->luminance[i]);
				ofSetColor(color);

				//draw circle
				ofCircle(this->x[i], this->y[i], this->radius[i]);

				//draw the title
				if (this->fill[i]) {
					color.setBrightness(fmod(color.getBrightness() + 127, 255));
					ofSetColor(color);
				}
				ofDrawBitmapString(this->name[i], this->x[i], this->y[i]);
			}
			ofPopStyle();
		}
	};
}

//--------------------------------------------------------------
void ofApp::update(){

}

using namespace ofxAssets;

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
