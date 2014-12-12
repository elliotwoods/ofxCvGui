#pragma once
#include "Base.h"
#include "ofxCvGui/ElementGroup.h"

namespace ofxCvGui {
	namespace Panels {
		class ElementCanvas : public Base {
		public:
			ElementCanvas();

			/// Elements which transform with the canvas
			ElementGroupPtr getCanvasElementGroup();
			
			/// Elements which are fixed above the canvas
			ElementGroupPtr getFixedElementGroup(); 

			const ofVec2f & getScrollPosition() const;
			void setScrollPosition(const ofVec2f &);
			const ofRectangle & getCanvasExtents() const;
		protected:
			void update();
			ElementGroupPtr canvasElements;
			ElementGroupPtr fixedElements;

			ofVec2f scrollPosition;
			ofRectangle canvasExtents;
		};
	}
}