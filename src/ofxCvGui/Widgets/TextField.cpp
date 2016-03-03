#include "TextField.h"

#include "ofxAssets.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		TextField::TextField(string caption) {
			this->setCaption(caption);
			this->setBounds(ofRectangle(5, 0, 100, 50));
			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				//draw caption
				auto & captionFont = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 12);
				captionFont.drawString(this->caption + " : ", 0, 15);
				
				//draw underline
				ofPushStyle();
				{
					const auto textFieldBounds = this->textField->getBounds();
					ofSetColor(100);
					ofSetLineWidth(1.0f);
					ofDrawLine(textFieldBounds.getBottomLeft(), textFieldBounds.getBottomRight());
				}
				ofPopStyle();
				
				//draw side line
				ofPushStyle();
				{
					ofSetLineWidth(1.0f);
					ofDrawLine(this->getWidth(), 0, this->getWidth(), 20);
				}
				ofPopStyle();
			};
			
			this->textField = make_shared<Utils::TextField>();
			this->textField->addListenersToParent(this);
			this->textField->setFont(ofxAssets::font(ofxCvGui::getDefaultTypeface(), 12));
			
			this->onBoundsChange += [this](BoundsChangeArguments & args) {
				this->textField->setBounds(ofRectangle(10, 20, args.localBounds.width - 20, 20));
			};
			this->onMouse += [this](MouseArguments & args) {
				if(args.takeMousePress(this)) {
					//if we clicked in here
					this->textField->focus();
				} else if (this->textField->isFocused() && !args.isLocal()) {
					this->textField->defocus();
				}
			};
		}
		
		//----------
		TextField::TextField(ofParameter<string> & parameter) :
		TextField(parameter.getName()) {
			this->textField->onTextChange += [this] (string text) {
				if(!this->disableTextFieldEvents) {
					this->parameter->set(text);
				}
			};
			this->parameter->addListener(this, &TextField::parameterCallback);
		}
		
		//----------
		void TextField::parameterCallback(string & text) {
			this->disableTextFieldEvents = true;
			this->textField->setText(text);
			this->disableTextFieldEvents = false;
		}
	}
}