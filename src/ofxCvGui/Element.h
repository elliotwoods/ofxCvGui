#pragma once
#include <functional>
#include <set>

#include "Types.h"
#include "Utils/Utils.h"
#include "Utils/Constants.h"

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofxLiquidEvent.h"

namespace ofxCvGui {
	class Element {
	public:
		enum LocalMouseState {
			Waiting = 1,
			Down = 2,
			Dragging = 6, // Dragging & Down != 0. So localMouseState & Down will return true if dragging
			ChildOwnsMouse = 8 // for when a child element owns the mouse
		};

		Element();
		virtual ~Element() { }
		
 		Element(const Element &) = delete;

		void update();
		
		void draw(const DrawArguments& arguments); // zoom is considered to act on the element's top-left local corner
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
		const ofRectangle & getBounds() const; // aka natural bounds, unzoomed bounds
		ofRectangle getLocalBounds() const;
		ofRectangle getBoundsInParent() const;
		float getWidth() const;
		float getHeight() const;

		void setZoom(float);
		float getZoom() const;

		ofMatrix4x4 getParentToLocalTransform() const;
		
		void setCaption(string caption);
		const string & getCaption() const;
		ofxLiquidEvent<string> onCaptionChange;
		
		ofxLiquidEvent<UpdateArguments> onUpdate;
		ofxLiquidEvent<DrawArguments> onDraw;
		ofxLiquidEvent<MouseArguments> onMouse;
		ofxLiquidEvent<KeyboardArguments> onKeyboard;
		ofxLiquidEvent<BoundsChangeArguments> onBoundsChange;
		ofxLiquidEvent<ZoomChangeArguments> onZoomChange;

		//utility events
		ofxLiquidEvent<MouseArguments> onMouseReleased;

		void setEnabled(bool);
		bool getEnabled() const;
		void enable();
		void disable();

		void setScissorEnabled(bool);

		void addListenersToParent(Element *, bool syncBoundsToParent = false);
		void addListenersToParent(shared_ptr<Element>, bool syncBoundsToParent = false);
		void removeListenersFromParent(Element *);
		void removeListenersFromParent(shared_ptr<Element>);

		void setCachedView(bool cachedViewEnabled);
		void markViewDirty();
	protected:
		void updateParentToLocalTransform();
		void allocateCachedView();
		void setHitTestOnBounds(bool);

		ofRectangle bounds; ///<bounds relative to parent
		float zoomFactor;
		ofMatrix4x4 parentToLocalTransform;

		string caption;
		bool enabled;
		bool enableScissor;
		bool enableHitTestOnBounds;
		LocalMouseState localMouseState;
		bool mouseOver;

		ofFbo * cachedView = 0;
		bool needsViewUpdate;
	};
	
	typedef shared_ptr<Element> ElementPtr;
	ElementPtr makeElement();
}