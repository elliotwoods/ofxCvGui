#include "ofxCvGui/ElementGroup.h"
namespace ofxCvGui {
	//----------
	template<typename T>
	ElementGroup_<T>::ElementGroup_() {
		this->onUpdate.addListener([this] (const UpdateArguments & args) {
			this->updateSet();
		}, this);
		this->onMouse.addListener([this] (const MouseArguments & args) {
			MouseArguments nonConstArgs = args;
			this->mouseActionSet(nonConstArgs);
		}, this);
		this->onKeyboard.addListener([this] (const KeyboardArguments & args) {
			KeyboardArguments nonConstArgs = args;
			this->keyboardActionSet(nonConstArgs);
		}, this);
		this->onDraw.addListener([this] (const DrawArguments & args) {
			DrawArguments nonConstArgs = args;
			this->drawSet(nonConstArgs);
		}, this);
	}

	//----------
	template<typename T>
	ElementGroup_<T>::~ElementGroup_() {
		this->onUpdate.removeListeners(this);
		this->onMouse.removeListeners(this);
		this->onKeyboard.removeListeners(this);
		this->onDraw.removeListeners(this);
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::updateSet() {
		for (auto & element : this->elements) {
			element->update();
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::mouseActionSet(MouseArguments& mouse) {
		for (auto & element : this->elements) {
			auto localArgs = MouseArguments(mouse, element->getBounds());
			element->mouseAction(localArgs);
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::keyboardActionSet(KeyboardArguments& keyboard) {
		for (auto & element : elements) {
			element->keyboardAction(keyboard);
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::add(ofPtr<T> addition) {
		this->elements.push_back(addition);
        ofRectangle bounds = this->getBounds();
		auto args = BoundsChangeArguments(bounds);
		this->onBoundsChange(args);
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::drop(ofPtr<T> element) {
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