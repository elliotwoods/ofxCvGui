#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		Heartbeat::Heartbeat(const string & caption
			, const GetFunction & getFunction
			, float coolOffPeriod)
		: getFunction(getFunction)
		, coolOffPeriod(coolOffPeriod)
		{
			init(caption);
			this->onUpdate += [this](ofxCvGui::UpdateArguments&) {
				this->update();
			};
		}

		//----------
		void Heartbeat::init(string caption) {
			this->setCaption(caption);
			this->setBounds(ofRectangle(0, 0, 100, 20));
			this->onDraw += [this](ofxCvGui::DrawArguments& args) {
				//draw caption
				{
					auto& font = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 12);
					auto text = this->caption + " : ";
					auto naturalBounds = font.getStringBoundingBox(text, 0, 0);
					font.drawString(text
						, args.localBounds.width - 20 - 5 - naturalBounds.getWidth()
						, 15);
				}


				auto timeSinceLastHeartbeat = std::chrono::system_clock::now() - this->lastHeartbeat;
				auto timeInSeconds = (float) std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceLastHeartbeat).count() / 1000.0f;

				//draw indicator
				ofPushStyle();
				{
					// too long ago
					if (timeInSeconds > this->coolOffPeriod) {
						ofSetLineWidth(1);
						ofNoFill();
						ofSetColor(100);
					}

					// fading
					else {
						ofFill();
						ofSetColor(
							ofMap(timeInSeconds
								, 0, this->coolOffPeriod
								, 255, 0)
						);
					}
					
					ofDrawCircle(this->getWidth() - 20, 10, 5);
				}
				ofPopStyle();

				// Draw the time
				if (this->isMouseOver() && timeInSeconds < ofGetElapsedTimef()) {
					ofPushStyle();
					{
						ofSetColor(150);
						auto& font = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 14);
						auto text = ofToString(timeInSeconds, 1) + "s";
						auto naturalBounds = font.getStringBoundingBox(text, 0, 0);
						font.drawString(text, args.localBounds.width - naturalBounds.width, 35);
					}
					ofPopStyle();
				}
			};

		}

		//----------
		void
		Heartbeat::update()
		{
			if (this->getFunction()) {
				this->lastHeartbeat = std::chrono::system_clock::now();
			}
		}
	}
}