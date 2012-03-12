#pragma once
#include "ofxCvGui/Types.h"
#include "ofxCvGui/Element.h"
#include "ofxCvGui/Panels/Base.h"
#include <vector>

using namespace std;

namespace ofxCvGui {
	typedef vector<ElementPtr> ElementSet;
	typedef vector<ElementPtr>::iterator ElementIterator;
	typedef vector<PanelPtr> PanelSet;
	typedef vector<PanelPtr>::iterator PanelIterator;

	template <typename T>
	class ElementGroup_ : public T {
	public:
		void update();
		void mouseAction(const MouseArguments& mouse);
		void keyboardAction(const KeyboardArguments& keyboard);
		void add(ofPtr<T>& addition);
		void drop(ofPtr<T>& element);
		void clear();

	protected:
		void drawSet(const DrawArguments& arguments);
		vector<ofPtr<T>> elements;
	};

	class ElementGroup : public ElementGroup_<Element> {
	protected:
		void drawElement(const DrawArguments& arguments) {
			this->drawSet(arguments);
		}
	};
}