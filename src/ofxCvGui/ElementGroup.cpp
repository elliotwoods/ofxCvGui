#include "ofxCvGui/ElementGroup.h"
namespace ofxCvGui {
	//----------
	template<typename T>
	void ElementGroup_<T>::update() {
		typename vector<ofPtr<T> >::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).update();		
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::mouseAction(MouseArguments& mouse) {
		typename vector<ofPtr<T> >::iterator it;
		for (auto element : this->elements) {
			auto localArgs = MouseArguments(mouse, (**it).getBounds());
			(*element).mouseAction(localArgs);
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::keyboardAction(KeyboardArguments& keyboard) {
		typename vector<ofPtr<T> >::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			(**it).keyboardAction(keyboard);
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::add(ofPtr<T>& addition) {
		this->elements.push_back(addition);
        ofRectangle bounds = this->getBounds();
		auto args = BoundsChangeArguments(bounds);
		this->onBoundsChange(args);
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::drop(ofPtr<T>& element) {
		typename vector<ofPtr<T> >::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			if (*it == element) {
				elements.erase(it);
				break;
			}

        ofRectangle bounds = this->getBounds();
		auto args = BoundsChangeArguments(bounds);
		this->onBoundsChange(args);
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::clear() {
		this->elements.clear();
        ofRectangle bounds;
		auto args = BoundsChangeArguments(bounds);
		this->onBoundsChange(args);
	}
	
	//----------
	template<typename T>
	void ElementGroup_<T>::drawSet(const DrawArguments& arguments) {
		typename vector<ofPtr<T> >::iterator it;
		for (it = elements.begin(); it != elements.end(); it++) {
            DrawArguments localArgs((*it)->getBounds(), arguments.chromeEnabled);
			(**it).draw(localArgs);
        }
	}

	template class ElementGroup_<Element>;
	template class ElementGroup_<Panels::Base>;
}