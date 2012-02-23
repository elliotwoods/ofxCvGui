#include "ofxCvGui/Types.h"
namespace ofxCvGui {
	//----------
	DrawArguments::DrawArguments(const ofRectangle& parentBounds, bool chromeEnabled) :
		parentBounds(parentBounds),
		chromeEnabled(chromeEnabled)
	{ }

	//----------
	MouseArguments::MouseArguments(const ofMouseEventArgs& mouseArgs, MouseActionType action, const ofRectangle& rectangle) :
		action(action),
		button(mouseArgs.button),
		global(mouseArgs.x, mouseArgs.y),
		local(global.x - rectangle.x, global.y - rectangle.y),
		localNormalised(local / ofVec2f(rectangle.width, rectangle.height))
	{ }

	//----------
	MouseArguments::MouseArguments(const MouseArguments& parentArguments, const ofRectangle& childBounds) :
		action(parentArguments.action),
		button(parentArguments.button),
		global(parentArguments.global),
		local(parentArguments.local - ofVec2f(childBounds.x, childBounds.y)),
		localNormalised(local / ofVec2f(childBounds.x, childBounds.y))
	{ }

	//----------
	KeyboardArguments::KeyboardArguments(const ofKeyEventArgs& keyboardArgs, KeyboardActionType action) :
		action(action),
		key(keyboardArgs.key)
	{ }
}