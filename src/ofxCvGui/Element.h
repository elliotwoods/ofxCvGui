#pragma once

#include "Types.h"
#include "Utils/Utils.h"
#include "Utils/Constants.h"

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofxLiquidEvent.h"

#include "ofVectorMath.h"

#include <functional>
#include <set>

namespace ofxCvGui {
	typedef std::shared_ptr<Element> ElementPtr;

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

		void setPosition(const glm::vec2 &);
		const ofRectangle & getBounds() const; // aka natural bounds, unzoomed bounds
		ofRectangle getLocalBounds() const;
		ofRectangle getBoundsInParent() const;
		float getWidth() const;
		float getHeight() const;

		void setZoom(float);
		float getZoom() const;

		glm::mat4x4 getParentToLocalTransform() const;
		
		void setCaption(std::string caption);
		const std::string & getCaption() const;
		ofxLiquidEvent<std::string> onCaptionChange;
		
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
		void addListenersToParent(std::shared_ptr<Element>, bool syncBoundsToParent = false);
		void removeListenersFromParent(Element *);
		void removeListenersFromParent(std::shared_ptr<Element>);

		void addChild(ElementPtr);
		void removeChild(ElementPtr);
		std::set<ElementPtr> getChildren() const;

		void setCachedView(bool cachedViewEnabled);
		void markViewDirty();
	protected:
		void updateParentToLocalTransform();
		void allocateCachedView();
		void setHitTestOnBounds(bool);

		ofRectangle bounds; ///<bounds relative to parent
		float zoomFactor;
		glm::mat4 parentToLocalTransform;

		std::string caption;
		bool enabled;
		bool enableScissor;
		bool enableHitTestOnBounds;
		LocalMouseState localMouseState;
		bool mouseOver;

		ofFbo * cachedView = 0;
		bool needsViewUpdate;

		std::set<ElementPtr> children;
	};
	
	ElementPtr makeElement();
}