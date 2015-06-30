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
			this->setBounds(ofRectangle(5, 0, 100, 30));
			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				//draw caption
				auto & captionFont = ofxAssets::font(ofxCvGui::defaultTypeface, 12);
				captionFont.drawString(this->caption + " : ", 0, 15);

				auto status = this->statusFunction();

				if (Indicator::colorMap->find(status) == Indicator::colorMap->end()) {
					ofLogError("ofxCvGui::Indicator") << "Your status has an invalid value. Maybe not initialised?";
					return;
				}

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
					ofSetColor(Indicator::colorMap->at(status));
				}
				ofCircle(this->getWidth() - 20, 10, 5);

				ofPopStyle();

				//draw side line
				ofPushStyle();
				ofSetLineWidth(1.0f);
				ofLine(this->getWidth(), 0, this->getWidth(), 20);
				ofPopStyle();
			};

			if (!Indicator::colorMap) {
				colorMap = new map<Status, ofColor>();
				colorMap->insert(pair<Status, ofColor>(Status::Clear, ofColor(0, 0)));
				colorMap->insert(pair<Status, ofColor>(Status::Good, ofColor(100, 200, 100)));
				colorMap->insert(pair<Status, ofColor>(Status::Warning, ofColor(100, 100, 0)));
				colorMap->insert(pair<Status, ofColor>(Status::Error, ofColor(100, 0, 0)));
			}
		}
	}
}