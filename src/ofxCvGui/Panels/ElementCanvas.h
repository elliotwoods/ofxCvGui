#pragma once
#include "Base.h"
#include "ofxCvGui/ElementGroup.h"
#include "ofxCvGui/ElementSlot.h"

#include "ofParameter.h"

namespace ofxCvGui {
	namespace Panels {
		class ElementCanvas : public Base {
		public:
			ElementCanvas();

			/// Elements which transform with the canvas
			ElementGroupPtr getCanvasElementGroup();
			
			/// Elements which are fixed above the canvas
			ElementGroupPtr getFixedElementGroup(); 

			void setScrollPosition(const glm::vec2 &);
			const glm::vec2 & getScrollPosition() const;

			const ofRectangle & getCanvasExtents() const;
		protected:
			void update();
			void callbackZoomChange(float &);

			ElementGroupPtr canvasElements;
			ElementGroupPtr fixedElements;
			ElementPtr zoomControl;

			ofParameter<float> zoom;
			glm::vec2 scrollPosition; // 0,0 means no scroll. +,+ means canvas moves up left
			glm::vec2 correctiveVelocity;

			ofRectangle canvasExtents;
		};

		shared_ptr<Panels::ElementCanvas> makeElementCanvas(string caption = "");
	}
}