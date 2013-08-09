#pragma once
#include <functional>

#include "Types.h"
#include "ofGraphics.h"
#include "Utils/LambdaStack.h"

namespace ofxCvGui {
	class Element {
	public:
		Element();
		virtual ~Element() { }
		void update();
		
		void draw(DrawArguments& arguments);
		void mouseAction(MouseArguments& mouse);
		void keyboardAction(KeyboardArguments& keyboard);

		void setBounds(const ofRectangle& bounds);
		const ofRectangle& getBounds() const;
		float getWidth() const;
		float getHeight() const;
		void setCaption(string caption);
		
		Utils::LambdaStack<UpdateArguments> onUpdate;
		Utils::LambdaStack<DrawArguments> onDraw;
		Utils::LambdaStack<MouseArguments> onMouse;
		Utils::LambdaStack<KeyboardArguments> onKeyboard;
		Utils::LambdaStack<BoundsChangeArguments> onBoundsChange;
		
		void enable();
		void disable();

	protected:
		ofRectangle bounds; ///<bounds relative to parent
		ofRectangle localBounds; ///<bounds for internal draw functions, i.e. x == y == 0
		string caption;
		bool enabled;
	};
	
	typedef ofPtr<Element> ElementPtr;
}