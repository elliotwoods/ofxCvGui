#include "ofxCvGui/ElementGroup.h"
namespace ofxCvGui {
	//----------
	template<typename T>
	void ElementGroup_<T>::update() {
		vector<ofPtr<T>>::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).update();		
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::mouseAction(const MouseArguments& mouse) {
		vector<ofPtr<T>>::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).mouseAction(MouseArguments(mouse, (**it).getBounds()));
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::keyboardAction(const KeyboardArguments& keyboard) {
		vector<ofPtr<T>>::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).keyboardAction(keyboard);
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::add(ofPtr<T>& addition) {
		this->elements.push_back(addition);
		this->boundsChange();
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::drop(ofPtr<T>& element) {
		vector<ofPtr<T>>::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			if (*it == element) {
				elements.erase(it);
				break;
			}

		this->boundsChange();
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::clear() {
		this->elements.clear();
		this->boundsChange();
	}
	
	//----------
	template<typename T>
	void ElementGroup_<T>::drawSet(const DrawArguments& arguments) {
		vector<ofPtr<T>>::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).draw(arguments);
	}

	template class ElementGroup_<Element>;
	template class ElementGroup_<Panels::Base>;
}