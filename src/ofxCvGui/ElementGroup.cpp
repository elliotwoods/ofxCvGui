#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	//----------
	template<typename T>
	ElementGroup_<T>::ElementGroup_() {
		this->onUpdate.addListener([this] (const UpdateArguments & args) {
			this->updateSet();
		}, this);
		this->onMouse.addListener([this] (MouseArguments & args) {
			this->mouseActionSet(args);
		}, this);
		this->onKeyboard.addListener([this] (const KeyboardArguments & args) {
			KeyboardArguments nonConstArgs = args;
			this->keyboardActionSet(nonConstArgs);
		}, this);
		this->onDraw.addListener([this] (const DrawArguments & args) {
			DrawArguments nonConstArgs = args;
			this->drawSet(nonConstArgs);
		}, this);
		this->onBoundsChange.addListener([this](const BoundsChangeArguments & args) {
			this->arrangeSet(args);
		}, this);
		this->onGroupChange.addListener([this]() {
			this->arrange();
		}, 10000, this);
	}

	//----------
	template<typename T>
	ElementGroup_<T>::~ElementGroup_() {
		this->onUpdate.removeListeners(this);
		this->onMouse.removeListeners(this);
		this->onKeyboard.removeListeners(this);
		this->onDraw.removeListeners(this);
		this->onBoundsChange.removeListeners(this);
		this->onGroupChange.removeListeners(this);
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
	void ElementGroup_<T>::mouseActionSet(MouseArguments& args) {
		//since these actions may change the element count,
		// we will cache the list of elements before
		auto cachedElements = this->elements;

		auto it = cachedElements.rbegin();
		for (; it != cachedElements.rend(); it++) {
			auto element = *it;
			element->mouseAction(args);
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::keyboardActionSet(KeyboardArguments& keyboard) {
		//since these actions may change the element count,
		// we will cache the list of elements before
		auto cachedElements = this->elements;

		for (auto & element : cachedElements) {
			element->keyboardAction(keyboard);
		}
	}

	//----------
	template<typename T>
	shared_ptr<T> ElementGroup_<T>::addBlank() {
		shared_ptr<T> newElement = shared_ptr<T>(new T);
		this->add(newElement);
		return newElement;
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::add(shared_ptr<T> addition) {
		this->elements.push_back(addition);
		this->onGroupChange.notifyListeners();
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::add(T * addition) {
		this->add(shared_ptr<T>(addition));
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::remove(shared_ptr<T> element) {
		typename vector<shared_ptr<T> >::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
			if (*it == element) {
				elements.erase(it);
				break;
			}

		this->onGroupChange.notifyListeners();
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::clear() {
		this->elements.clear();
		this->onGroupChange.notifyListeners();
	}
	
	//----------
	template<typename T>
	const vector<shared_ptr<T>> & ElementGroup_<T>::getElements() const {
		return this->elements;
	}

	//----------
	template<typename T>
	vector<shared_ptr<T>> & ElementGroup_<T>::getElements() {
		return this->elements;
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::layoutGridVertical(float spacing) {
		const auto localBounds = this->getLocalBounds();
		const auto step = (localBounds.getHeight() - spacing) / (float) this->elements.size();
		const auto width = localBounds.getWidth();
		float y = spacing;
		for (auto element : this->elements) {
			element->setBounds(ofRectangle(0.0f, y, width, step - spacing));
			y += step;
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::layoutGridHorizontal(float spacing) {
		const auto localBounds = this->getLocalBounds();
		const auto step = (localBounds.getWidth() - spacing) / (float) this->elements.size();
		const auto height = localBounds.getHeight();
		float x = spacing;
		for (auto element : this->elements) {
			element->setBounds(ofRectangle(0.0f, x, step - spacing, height));
			x += step;
		}
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::drawSet(const DrawArguments & arguments) {
		typename vector<shared_ptr<T> >::iterator it;
		for (it = elements.begin(); it != elements.end(); it++) {
			(**it).draw(arguments);
        }
	}

	//----------
	template<typename T>
	void ElementGroup_<T>::arrangeSet(const BoundsChangeArguments &) {
		typename vector<shared_ptr<T> >::iterator it;
		for (it = elements.begin(); it != elements.end(); it++) {
			(**it).arrange();
		}
	}
	
	//----------
	ElementGroupPtr makeElementGroup() {
		return make_shared<ElementGroup>();
	}

	template class ElementGroup_<Element>;
	template class ElementGroup_<Panels::Base>;
}