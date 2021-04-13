#pragma once
#include "ofRectangle.h"
#include "ofEvents.h"
#include "ofTypes.h"
#include "ofVectorMath.h"

#include "ofxCvGui/Utils/Constants.h"

namespace ofxCvGui {
	//-------------
	class Element;

	//----------
    struct UpdateArguments {
        
    };
    
	//----------
	struct DrawArguments {
		bool chromeEnabled;
		ofRectangle naturalBounds; /// bounds within parent as stored locally. before scaling
		glm::mat4 globalTransform; /// warning : global properties can be relative to an fbo if within a cached view element
		float globalScale;
		ofRectangle localBounds;
		ofRectangle globalBounds;
	};

	//----------
    class InputArguments {
    public:
        InputArguments(const std::shared_ptr<void>& currentPanel) : currentPanel(currentPanel) { }
        const std::shared_ptr<void> currentPanel;
        
        bool checkCurrentPanel(void * panel) {
            return currentPanel.get() == panel;
        }
    };
    
	//----------
	class OFXCVGUI_API_ENTRY MouseArguments : public InputArguments {
    public:
        enum Action {
            Pressed = 1 << 0,
			Released = 1 << 1,
			Moved = 1 << 2,
			Dragged = 1 << 3,
			DoubleClick = 1 << 4,
			Scrolled = 1 << 5
        };
        
		MouseArguments(); //local
		MouseArguments(const ofMouseEventArgs& mouseArgs
			, Action action
			, const ofRectangle& rectangle
			, const std::shared_ptr<void>& currentPanel
			, void * owner
			, const glm::vec2 & cached = glm::vec2()); ///global
		
		bool isLocal() const; 
		bool isTaken() const;

		/// If the click is local and available then take it and return true, else return false
		bool takeMousePress(void * element);
		bool takeMousePress(std::shared_ptr<Element>);

		/// Not taken, or is something other than a mouse down action
		bool mightStillBeUseful() const;

		/// Only use this is you know what you're doing. This is for manually marking a mouse take
		void forceMouseTake(void * element);

		bool isDragging(void * element) const;
		bool isDragging(std::shared_ptr<Element>) const;
		bool isDoubleClicked(void * element) const;
		bool isDoubleClicked(std::shared_ptr<Element>) const;

		void * getOwner() const;

		Action action;
		int button;
		glm::vec2 global;
		glm::vec2 local;
		glm::vec2 localNormalized; ///<Texture coordinates
		glm::vec2 movement;
		glm::vec2 scroll;

		friend std::ostream& operator<<(std::ostream&, const MouseArguments &);
	protected:
		void * takenBy;
	};

	//----------
	class OFXCVGUI_API_ENTRY KeyboardArguments : public InputArguments {
    public:
        enum Action {
            Pressed, Released
        };
        
		KeyboardArguments(const ofKeyEventArgs& keyboardArgs
			, Action action
			, std::shared_ptr<void> currentPanel);
        
		const Action action;
		const int key;
	};
	
	//----------
	class OFXCVGUI_API_ENTRY BoundsChangeArguments {
	public:
		BoundsChangeArguments(const ofRectangle & bounds);
		const ofRectangle bounds;
		const ofRectangle localBounds;
	};

	//----------
	class OFXCVGUI_API_ENTRY FilesDraggedArguments {
	public:
		FilesDraggedArguments(const glm::vec2 & localPosition
			, const glm::vec2 & globalPosition
			, const std::vector<std::string> & files);

		const glm::vec2 localPosition;
		const glm::vec2 globalPosition;
		const std::vector<std::string> files;
	};

	//----------
	struct OFXCVGUI_API_ENTRY ZoomChangeArguments {
		float oldZoom;
		float newZoom;
	};
}