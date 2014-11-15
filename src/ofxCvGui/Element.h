#pragma once
#include <functional>
#include <set>

#include "Types.h"
#include "Utils/Utils.h"
#include "Utils/Constants.h"

#include "ofGraphics.h"
#include "../../../addons/ofxLiquidEvent/src/ofxLiquidEvent.h"

#define OFXCVGUI_MAKE_ELEMENT_HEADER(T, ...) static shared_ptr<T> make(__VA_ARGS__)
#define OFXCVGUI_MAKE_ELEMENT_BODY(T, ...) return shared_ptr<T>(new T(__VA_ARGS__));

namespace ofxCvGui {
	class Element {
	public:
		enum LocalMouseState {
			Waiting,
			Down,
			Dragging
		};

		Element();
		virtual ~Element() { }
		void update();
		
		void draw(const DrawArguments& arguments);
		void mouseAction(MouseArguments& mouse);
		void keyboardAction(KeyboardArguments& keyboard);

		void clearMouseState();
		LocalMouseState getMouseState() const;
		bool isMouseDown() const;
		bool isMouseDragging() const;
		bool isMouseOver() const;

		void setBounds(const ofRectangle& bounds);
		void setWidth(float);
		void setHeight(float);
		void arrange();

		void setPosition(const ofVec2f&);
		const ofRectangle & getBounds() const;
		const ofRectangle getLocalBounds() const;
		float getWidth() const;
		float getHeight() const;
		
		void setCaption(string caption);
		ofxLiquidEvent<string> onCaptionChange;
		
		ofxLiquidEvent<UpdateArguments> onUpdate;
		ofxLiquidEvent<DrawArguments> onDraw;
		ofxLiquidEvent<MouseArguments> onMouse;
		ofxLiquidEvent<KeyboardArguments> onKeyboard;
		ofxLiquidEvent<BoundsChangeArguments> onBoundsChange;

		//utility events
		ofxLiquidEvent<MouseArguments> onMouseReleased;

		void setEnabled(bool);
		bool getEnabled() const;
		void enable();
		void disable();

		void addListenersToParent(Element *, bool syncBoundsToParent = false);
		void addListenersToParent(shared_ptr<Element>, bool syncBoundsToParent = false);
		void removeListenersFromParent(Element *);
		void removeListenersFromParent(shared_ptr<Element>);
	protected:
		void setScissor(bool);
		ofRectangle bounds; ///<bounds relative to parent
		ofRectangle localBounds; ///<bounds for internal draw functions, i.e. x == y == 0
		string caption;
		bool enabled;
		bool enableScissor;
		LocalMouseState localMouseState;
		bool mouseOver;
		set<shared_ptr<Element>> listeningElements; ///<other elements which react to this elements events
	};
	
	typedef shared_ptr<Element> ElementPtr;
}