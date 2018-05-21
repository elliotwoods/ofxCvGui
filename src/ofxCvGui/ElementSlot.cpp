#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	//----------
	ElementSlot::ElementSlot() {
		this->enableHitTestOnBounds = false;

		this->onUpdate += [this](ofxCvGui::UpdateArguments & args) {
			if (this->element) {
				this->element->update();
			}
		};

		this->onDraw += [this](ofxCvGui::DrawArguments & args) {
			if (this->element) {
				this->element->draw(args);
			}
		};

		this->onMouse += [this](ofxCvGui::MouseArguments & args) {
			if (this->element) {
				this->element->mouseAction(args);
			}
		};

		this->onKeyboard += [this](ofxCvGui::KeyboardArguments & args) {
			if (this->element) {
				this->element->keyboardAction(args);
			}
		};

		this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments & args) {
			if (this->element) {
				this->element->setBounds(args.localBounds);
			}
		};
	}

	//----------
	void ElementSlot::setElement(shared_ptr<Element> element) {
		this->element = element;
	}

	//----------
	shared_ptr<Element> ElementSlot::getElement() const {
		return this->element;
	}

	//----------
	void ElementSlot::clearElement() {
		this->element.reset();
	}
}