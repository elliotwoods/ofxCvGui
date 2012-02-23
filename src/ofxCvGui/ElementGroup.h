#pragma once
#include "ofxCvGui/ElementPtr.h"
#include <vector>

using namespace std;

namespace ofxCvGui {
	typedef vector<ElementPtr> ElementSet;
	typedef vector<ElementPtr>::iterator ElementIterator;
	typedef vector<PanelPtr> PanelSet;
	typedef vector<PanelPtr>::iterator PanelIterator;

	template <typename T>
	class ElementGroup_ : public T {
		typedef vector<ElementPtr_<T>> ElementIterator_;
	public:
		void update();
		void mouseAction(const MouseArguments& mouse);
		void keyboardAction(const KeyboardArguments& keyboard);
	protected:
		void customDraw(const DrawArguments& arguments);
		vector<ElementPtr_<T>> elements;
	};

	typedef ElementGroup_<Element> ElementsGroup;
	typedef ElementGroup_<Panels::Base> PanelGroup;
}