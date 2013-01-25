#pragma once
#include "Types.h"
#include "ofGraphics.h"

namespace ofxCvGui {
	class Element {
	public:
		virtual ~Element() { }
		void update();
		void draw(const DrawArguments& arguments); //to set your draw, listen to the onDraw event
		void mouseAction(const MouseArguments& mouse);
		void keyboardAction(const KeyboardArguments& keyboard);

		void setBounds(const ofRectangle& bounds);
		const ofRectangle& getBounds() const;
		float getWidth() const;
		float getHeight() const;

		void setCaption(string caption);
        
        ofEvent<UpdateArguments> onUpdate;
        ofEvent<DrawArguments> onDraw;
        ofEvent<ofRectangle> onBoundsChange;
        ofEvent<MouseArguments> onMouseAction;
        ofEvent<KeyboardArguments> onKeyboardAction;
        
	protected:
		ofRectangle bounds; ///<bounds relative to parent
		ofRectangle localBounds; ///<bounds for internal draw functions, i.e. x == y == 0
		string caption;
	};
	typedef ofPtr<Element> ElementPtr;
}