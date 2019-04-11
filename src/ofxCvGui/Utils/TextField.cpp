#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Utils {
		//----------
		TextField::TextField() {
			this->textInputField.setup(false);
			
			this->onKeyboard += [this](ofxCvGui::KeyboardArguments & args) {
				ofKeyEventArgs ofArgs;
				ofArgs.key = args.key;
				switch (args.action) {
				case ofxCvGui::KeyboardArguments::Action::Pressed:
					ofArgs.type = ofKeyEventArgs::Type::Pressed;
					this->textInputField.keyPressed(ofArgs);
					break;
				case ofxCvGui::KeyboardArguments::Action::Released:
					ofArgs.type = ofKeyEventArgs::Type::Released;
					this->textInputField.keyReleased(ofArgs);
					break;
				}
			};

			this->onMouse += [this](ofxCvGui::MouseArguments & args) {
				ofMouseEventArgs ofArgs;
				(glm::vec2&)ofArgs = args.local;

				ofArgs.button = args.button;

				switch (args.action) {
				case ofxCvGui::MouseArguments::Action::Pressed:
					if (args.takeMousePress(this)) {
						ofArgs.type = ofMouseEventArgs::Type::Pressed;
						this->textInputField.mousePressed(ofArgs);
					}
					break;
				case ofxCvGui::MouseArguments::Action::Dragged:
					ofArgs.type = ofMouseEventArgs::Type::Dragged;
					this->textInputField.mouseDragged(ofArgs);
					break;
				case ofxCvGui::MouseArguments::Action::Released:
					ofArgs.type = ofMouseEventArgs::Type::Released;
					this->textInputField.mouseReleased(ofArgs);
					break;
				}
			};

			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				if (!this->hintText.empty() && this->textInputField.text.empty()) {
					ofPushStyle();
					ofSetColor(100);
					auto fontRenderer = this->textInputField.getFontRenderer();
					auto typedFontRenderer = dynamic_cast<ofxTextInput::TypedFontRenderer *>(fontRenderer);
					if (typedFontRenderer) {
						typedFontRenderer->drawString(this->hintText, this->textInputField.getHorizontalPadding(), fontRenderer->getLineHeight() + this->textInputField.getVerticalPadding());
					}
					ofPopStyle();
				}
				this->textInputField.draw();
			};

			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments & args) {
				this->textInputField.bounds = args.localBounds;
			};

			ofAddListener(this->textInputField.onTextChange, this, &TextField::callbackTextChanged);
			ofAddListener(this->textInputField.onHitReturn, this, &TextField::callbackHitReturn);
		}

		//----------
		TextField::~TextField() {
			ofRemoveListener(this->textInputField.onTextChange, this, &TextField::callbackTextChanged);
			ofRemoveListener(this->textInputField.onHitReturn, this, &TextField::callbackHitReturn);
		}
		
		//----------
		void TextField::setHintText(const string & hintText) {
			this->hintText = hintText;
		}

		//----------
		const string & TextField::getText() const {
			return this->textInputField.text;
		}

		//----------
		void TextField::setText(const string & text) {
			this->textInputField.text = text;
			auto textCopy = text;
			this->onTextChange(textCopy);
		}

		//----------
		void TextField::setFont(ofTrueTypeFont & font) {
			this->textInputField.setFont(font);
		}

		//----------
		void TextField::clearText() {
			this->textInputField.clear();
		}

		//----------
		void TextField::focus() {
			this->textInputField.beginEditing();
		}


		//----------
		void TextField::defocus() {
			this->textInputField.endEditing();
		}
		
		//----------
		bool TextField::isFocused() const {
			return this->textInputField.isEditing();
		}

		//----------
		ofxTextInputField & TextField::getTextInputField() {
			return this->textInputField;
		}

		//----------
		void TextField::callbackTextChanged(string & text) {
			this->onTextChange(text);
		}

		//----------
		void TextField::callbackHitReturn(string & text) {
			this->onHitReturn(text);
		}
	}
}