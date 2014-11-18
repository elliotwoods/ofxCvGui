#pragma once
#include "ofxCvGui/Types.h"
#include "ofxCvGui/Element.h"
#include "ofxCvGui/Panels/Base.h"
#include <vector>

using namespace std;

namespace ofxCvGui {
	typedef vector<ElementPtr> ElementSet;
	typedef vector<ElementPtr>::iterator ElementIterator;

	template <typename T>
	class ElementGroup_ : public T {
	public:
		ElementGroup_();
		virtual ~ElementGroup_();
		void add(shared_ptr<T> addition);
		shared_ptr<T> addBlank();
		void drop(shared_ptr<T> element);
		void clear();
		vector<shared_ptr<T> > & getElements();

		void layoutGridVertical(float spacing = 0.0f);
		void layoutGridHorizontal(float spacing = 0.0f);
	protected:
		void updateSet();
		void mouseActionSet(MouseArguments& mouse);
		void keyboardActionSet(KeyboardArguments& keyboard);
		void drawSet(const DrawArguments& arguments);
		vector<shared_ptr<T> > elements;
	};

	class ElementGroup : public ElementGroup_<Element> {
	protected:
		void drawElement(const DrawArguments& arguments) {
			this->drawSet(arguments);
		}
	};

	typedef shared_ptr<ElementGroup> ElementGroupPtr;
}