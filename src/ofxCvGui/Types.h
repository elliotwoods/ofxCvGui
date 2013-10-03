#pragma once
#include "ofRectangle.h"
#include "ofEvents.h"
#include "ofTypes.h"

namespace ofxCvGui {
	//----------
    struct UpdateArguments {
        
    };
    
	//----------
	struct DrawArguments {
		DrawArguments(const ofRectangle& parentBounds, const ofRectangle& globalBounds, bool chromeEnabled);
        
		const ofRectangle parentBounds;
		const ofRectangle globalBounds;
		const bool chromeEnabled;
	};

	//----------
    class InputArguments {
    public:
        InputArguments(const ofPtr<void>& currentPanel) : currentPanel(currentPanel) { }
        const ofPtr<void> currentPanel;
        
        bool checkCurrentPanel(void * panel) {
            return currentPanel.get() == panel;
        }
    };
    
	//----------
	class MouseArguments : public InputArguments {
    public:
        enum Action {
            Pressed, Released, Moved, Dragged
        };
        
		MouseArguments(const ofMouseEventArgs& mouseArgs, Action action, const ofRectangle& rectangle, const ofPtr<void>& currentPanel, const ofVec2f& cached = ofVec2f()); ///global
		MouseArguments(const MouseArguments& parentArguments, const ofRectangle& childBounds); ///local
		
		bool isLocal() const; 
		bool isLocalPressed() const;

		const Action action;
		const int button;
		const ofVec2f global;
		const ofVec2f local;
		const ofVec2f localNormalised; ///<Texture coordinates
        const ofVec2f movement;
	};

	//----------
	class KeyboardArguments : public InputArguments {
    public:
        enum Action {
            Pressed, Released
        };
        
		KeyboardArguments(const ofKeyEventArgs& keyboardArgs, Action action, ofPtr<void> currentPanel);
        
		const Action action;
		const int key;
	};
	
	//----------
	class BoundsChangeArguments {
	public:
		BoundsChangeArguments(const ofRectangle & bounds);
		const ofRectangle bounds;
	};
}