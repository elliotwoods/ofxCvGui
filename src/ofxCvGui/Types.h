#pragma once
#include "ofRectangle.h"
#include "ofEvents.h"
#include "ofTypes.h"

namespace ofxCvGui {
	struct DrawArguments {
		DrawArguments(const ofRectangle& parentBounds, bool chromeEnabled);
		const ofRectangle parentBounds;
		const bool chromeEnabled;
	};

	enum MouseActionType {
		MousePressed, MouseReleased, MouseMoved, MouseDragged
	};

	struct MouseArguments {
		MouseArguments(const ofMouseEventArgs& mouseArgs, MouseActionType action, const ofRectangle& rectangle); ///global
		MouseArguments(const MouseArguments& parentArguments, const ofRectangle& childBounds); ///local
		
		const MouseActionType action;
		const int button;
		const ofVec2f global;
		const ofVec2f local;
		const ofVec2f localNormalised; ///<Texture coordinates
	};

	enum KeyboardActionType {
		KeyPressed, KeyReleased
	};

	struct KeyboardArguments {
		KeyboardArguments(const ofKeyEventArgs& keyboardArgs, KeyboardActionType action);
		const KeyboardActionType action;
		const int key;
	};
}