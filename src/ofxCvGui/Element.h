#pragma once
#include "Types.h"
#include "ofGraphics.h"

namespace ofxCvGui {
	class Element {
	public:
		virtual ~Element() { }
		virtual void update() { };
		void draw(const DrawArguments& arguments); //override customDraw instead
		virtual void mouseAction(const MouseArguments& mouse) { };
		virtual void keyboardAction(const KeyboardArguments& keyboard) { };

		void setBounds(const ofRectangle& bounds);
		const ofRectangle& getBounds();

		void setCaption(string caption);
	protected:
		virtual void customDraw(const DrawArguments& arguments) = 0; ///<override here to draw something
		virtual void boundsChange() { }; ///<override here if you want to cache something based on element's bounds
		ofRectangle bounds;
		string caption;
	};
}