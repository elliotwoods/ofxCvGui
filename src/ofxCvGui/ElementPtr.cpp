#include "ofxCvGui/ElementPtr.h"
namespace ofxCvGui {
	//-----------
	template<typename T>
	ElementPtr_<T>::ElementPtr_() {
		this->element = 0;
		this->localAllocation = false;
	}

	//-----------
	template<typename T>
	ElementPtr_<T>::ElementPtr_(const ElementPtr_<T>& other) {
		this->element = other.element;
		this->localAllocation = false;
	}

	//-----------
	template<typename T>
	ElementPtr_<T>::~ElementPtr_() {
		if (localAllocation)
			delete element;
	}
	
	//-----------
	template<typename T>
	T& ElementPtr_<T>::operator*() {
		return *element;
	}
	
	//-----------
	template<typename T>
	ElementPtr_<T>& ElementPtr_<T>::operator=(T* element) {
		this->element = element;
		localAllocation = true;
		return *this;
	}

	template class ElementPtr_<Element>;
	template class ElementPtr_<Panels::Base>;
}