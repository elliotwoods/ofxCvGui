#include "pch_ofxCvGui.h"

using namespace std;

namespace ofxCvGui {
#pragma mark MouseArguments
	//----------
	MouseArguments::MouseArguments(const ofMouseEventArgs& mouseArgs
		, Action action
		, const ofRectangle& rectangle
		, const shared_ptr<void>& currentPanel
		, void * takenBy
		, const glm::vec2 & cached) :

		action(action),
		button(mouseArgs.button),
		global(mouseArgs.x, mouseArgs.y),
		local(mouseArgs.x - rectangle.x, mouseArgs.y - rectangle.y),
		localNormalized(local / glm::vec2(rectangle.width, rectangle.height)),
        movement(action == Dragged ? global - cached : glm::vec2()),
		takenBy(takenBy),
        InputArguments(currentPanel)
	{ }

	//----------
	bool MouseArguments::isLocal() const {
		ofRectangle testBounds(0, 0, 1, 1);
		return testBounds.inside(localNormalized);
	}

	//----------
	bool MouseArguments::takeMousePress(void * element) {
		auto local = this->isLocal();
		auto gotClick = (action == Pressed) && local && (! this->isTaken() || this->getOwner() == element);
		if (gotClick) {
			this->forceMouseTake(element);
		}
		return gotClick;
	}

	//----------
	bool MouseArguments::takeMousePress(shared_ptr<Element> element) {
		return this->takeMousePress(element.get());
	}

	//----------
	bool MouseArguments::isTaken() const {
		return (bool) (this->takenBy);
	}

	//----------
	bool MouseArguments::mightStillBeUseful() const {
		return (!this->isTaken()) || action != Pressed;
	}

	//----------
	void MouseArguments::forceMouseTake(void * element) {
		this->takenBy = element;
	}

	//----------
	bool MouseArguments::isDragging(void * element) const {
		return this->getOwner() == element && this->action == MouseArguments::Action::Dragged;
	}

	//----------
	bool MouseArguments::isDragging(shared_ptr<Element> element) const {
		return this->isDragging((void*)element.get());
	}

	//----------
	bool MouseArguments::isDoubleClicked(void * element) const {
		return this->getOwner() == element && this->action == MouseArguments::Action::DoubleClick;
	}

	//----------
	bool MouseArguments::isDoubleClicked(shared_ptr<Element> element) const {
		return this->isDoubleClicked((void*)element.get());
	}

	//----------
	void * MouseArguments::getOwner() const {
		return this->takenBy;
	}

	//----------
	ostream& operator<<(ostream& os, const MouseArguments & args) {
		os << "[MouseAction : a(" << args.action << "), b(" << args.button << "), g(" << args.global << "), l(" << args.local << "), ln(" << args.localNormalized << "), m(" << args.movement << "), t(" << (args.isTaken() ? "X" : " ") << ")]";
		return os;
	}

#pragma mark KeyboardArguments
	//----------
	KeyboardArguments::KeyboardArguments(const ofKeyEventArgs& keyboardArgs, Action action, shared_ptr<void> currentPanel) :
		action(action),
		key(keyboardArgs.key),
        InputArguments(currentPanel)
	{ }
	
#pragma mark BoundsChangeArguments
	//----------
	BoundsChangeArguments::BoundsChangeArguments(const ofRectangle & bounds) :
		bounds(bounds),
		localBounds(0.0f, 0.0f, bounds.width, bounds.height)
	{ }

#pragma mark FilesDraggedArguments
	//----------
	FilesDraggedArguments::FilesDraggedArguments(const glm::vec2 & localPosition
		, const glm::vec2 & globalPosition
		, const vector<string> & files) :
		localPosition(localPosition), globalPosition(globalPosition), files(files)
	{ }
}