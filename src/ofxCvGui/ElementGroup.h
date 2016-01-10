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
		const vector<shared_ptr<T> > & getElements() const;
		vector<shared_ptr<T> > & getElements();

		void layoutGridVertical(float spacing = 0.0f);
		void layoutGridHorizontal(float spacing = 0.0f);

		ofxLiquidEvent<void> onGroupChange;
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
}