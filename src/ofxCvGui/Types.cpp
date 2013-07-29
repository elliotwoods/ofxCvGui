#include "ofxCvGui/Types.h"
namespace ofxCvGui {
#pragma mark DrawArguments
	//----------
	DrawArguments::DrawArguments(const ofRectangle& parentBounds, bool chromeEnabled) :
		parentBounds(parentBounds),
		chromeEnabled(chromeEnabled)
	{ }

#pragma mark MouseArguments
	//----------
	MouseArguments::MouseArguments(const ofMouseEventArgs& mouseArgs, Action action, const ofRectangle& rectangle, const ofPtr<void>& currentPanel, const ofVec2f& cached) :
		action(action),
		button(mouseArgs.button),
		global(mouseArgs.x, mouseArgs.y),
		local(mouseArgs.x - rectangle.x, mouseArgs.y - rectangle.y),
		localNormalised(local / ofVec2f(rectangle.width, rectangle.height)),
        movement(action == Dragged ? global - cached : ofVec2f()),
        InputArguments(currentPanel)
	{ }

	//----------
	MouseArguments::MouseArguments(const MouseArguments& parentArguments, const ofRectangle& childBounds) :
        action(parentArguments.action),
        button(parentArguments.button),
        global(parentArguments.global),
        local(parentArguments.local - ofVec2f(childBounds.x, childBounds.y)),
        localNormalised(local / ofVec2f(childBounds.width, childBounds.height)),
        movement(parentArguments.movement),
        InputArguments(parentArguments.currentPanel)
	{ }

	//----------
	bool MouseArguments::isLocalPressed() const {
		ofRectangle testBounds(0, 0, 1, 1);
		return action == Action::Pressed && testBounds.inside(localNormalised);
	}

#pragma mark KeyboardArguments
	//----------
	KeyboardArguments::KeyboardArguments(const ofKeyEventArgs& keyboardArgs, Action action, ofPtr<void> currentPanel) :
		action(action),
		key(keyboardArgs.key),
        InputArguments(currentPanel)
	{ }
	
#pragma mark BoundsChangeArguments
	//----------
	BoundsChangeArguments::BoundsChangeArguments(const ofRectangle & bounds) :
		bounds(bounds)
	{ }
}