#include "Indicator.h"
#include "../Utils/Constants.h"
#include "ofxAssets.h"
namespace ofxCvGui {
	namespace Widgets {
		//----------
		map<Indicator::Status, ofColor> * Indicator::colorMap = nullptr;

		//----------
		Indicator::Indicator(string caption, StatusFunction statusFunction) :
			statusFunction(statusFunction) {
			init(caption);
		}
		//----------
		void Indicator::init(string caption) {
			this->setCaption(caption);
			this->setBounds(ofRectangle(0, 0, 100, 20));
			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				//draw caption
				auto & captionFont = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 12);
				captionFont.drawString(this->caption + " : ", 0, 15);

				auto status = this->statusFunction();

				//draw indicator
				ofPushStyle();

				if (status == Status::Clear) {
					ofSetLineWidth(1);
					ofNoFill();
					ofSetColor(100);
				}
				else {
					ofSetLineWidth(0);
					ofFill();
					if (Indicator::colorMap->find(status) == Indicator::colorMap->end()) {
						ofLogError("ofxCvGui::Indicator") << "Your status has an invalid value. Maybe not initialised?";
					}
					else {
						ofSetColor(Indicator::colorMap->at(status));
					}
				}
				ofDrawCircle(this->getWidth() - 20, 10, 5);

				ofPopStyle();
			};

			if (!Indicator::colorMap) {
				colorMap = new map<Status, ofColor>();
				colorMap->insert(pair<Status, ofColor>(Status::Good, ofColor(100, 200, 100)));
				colorMap->insert(pair<Status, ofColor>(Status::Good, ofColor(100, 200, 100)));
				colorMap->insert(pair<Status, ofColor>(Status::Warning, ofColor(255, 255, 0)));
				colorMap->insert(pair<Status, ofColor>(Status::Error, ofColor(255, 100, 100)));
			}
		}
	}
}