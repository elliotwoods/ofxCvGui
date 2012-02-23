#include "ofxCvGui/ElementGroup.h"
namespace ofxCvGui {
	//----------
	void ElementGroup::update() {
		ElementIterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).update();		
	}

	//----------
	void ElementGroup::mouseAction(const MouseArguments& mouse) {
		ElementIterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).mouseAction(MouseArguments(mouse, (**it).getBounds()));
	}

	//----------
	void ElementGroup::keyboardAction(const KeyboardArguments& keyboard) {
		ElementIterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).keyboardAction(keyboard);
	}

	//----------
	void ElementGroup::customDraw(const DrawArguments& arguments) {
		ElementIterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).draw(arguments);
	}
}