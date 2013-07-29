#include "ofxCvGui/ElementGroup.h"
namespace ofxCvGui {
	//----------
	template<typename T>
	ElementGroup_<T>::ElementGroup_() {
		this->onDraw.addListener([this] (const DrawArguments & args) {
			this->drawSet(args);
		}, this);
	}

	//----------
	template<typename T>
	ElementGroup_<T>::~ElementGroup_() {
		this->onDraw.removeListeners(this);
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::update() {
		for (auto & element : this->elements) {
			element->update();
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::mouseAction(MouseArguments& mouse) {
		for (auto & element : this->elements) {
			auto localArgs = MouseArguments(mouse, element->getBounds());
			element->mouseAction(localArgs);
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::keyboardAction(KeyboardArguments& keyboard) {
		for (auto & element : elements) {
			element->keyboardAction(keyboard);
		}
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
	vector<ofPtr<T>> & ElementGroup_<T>::getElements() {
		return this->elements;
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