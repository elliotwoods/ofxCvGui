#include "ofxCvGui/ElementGroup.h"
namespace ofxCvGui {
	//----------
	template<typename T>
	void ElementGroup_<T>::update() {
		ElementIterator_ it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).update();		
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::mouseAction(const MouseArguments& mouse) {
		ElementIterator_ it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).mouseAction(MouseArguments(mouse, (**it).getBounds()));
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::keyboardAction(const KeyboardArguments& keyboard) {
		ElementIterator_ it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).keyboardAction(keyboard);
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::customDraw(const DrawArguments& arguments) {
		ElementIterator_ it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).draw(arguments);
	}
}