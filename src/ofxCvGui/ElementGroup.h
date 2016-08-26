#pragma once
#include "ofxCvGui/Types.h"
#include "ofxCvGui/Element.h"
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
		void add(T * addition);
		shared_ptr<T> addBlank();
		void remove(shared_ptr<T> element);
		void clear();
		const vector<shared_ptr<T> > & getElements() const;
		vector<shared_ptr<T> > & getElements();

		void layoutGridVertical(float spacing = 0.0f);
		void layoutGridHorizontal(float spacing = 0.0f);

		ofxLiquidEvent<void> onGroupChange;

		template<typename ElementType>
		bool isLastElementOfType() {
			if (this->elements.empty()) {
				return false;
			}
			if (dynamic_pointer_cast<ElementType>(this->elements.back())) {
				return true;
			}
			else {
				return false;
			}
		}
	protected:
		void updateSet();
		void mouseActionSet(MouseArguments &);
		void keyboardActionSet(KeyboardArguments &);
		void drawSet(const DrawArguments &);
		void arrangeSet(const BoundsChangeArguments &);
		vector<shared_ptr<T> > elements;
	};

	class ElementGroup : public ElementGroup_<Element> {
	protected:
		void drawElement(const DrawArguments& arguments) {
			this->drawSet(arguments);
		}
	};

	typedef shared_ptr<ElementGroup> ElementGroupPtr;
	ElementGroupPtr makeElementGroup();
}