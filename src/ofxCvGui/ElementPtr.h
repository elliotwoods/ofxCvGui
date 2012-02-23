#pragma once
#include "ofxCvGui/Element.h"
#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	template<typename T>
	struct ElementPtr_ {
		ElementPtr_();
		ElementPtr_(const ElementPtr_<T>& other);
		~ElementPtr_();

		T& operator*();
		ElementPtr_<T>& operator=(T* element);

	private:
		T* element;
		bool localAllocation;
	};

	typedef ElementPtr_<Element> ElementPtr;
	typedef ElementPtr_<Panels::Base> PanelPtr;
}