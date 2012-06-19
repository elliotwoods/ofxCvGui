#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Assets.h"

#include "ofNode.h"
#include "ofxGrabCam.h"

namespace ofxCvGui {
	namespace Panels {
		class Node : public Panels::Base {
		public:
			Node() { }
			Node(ofNode & node);
			
			//camera
			ofxGrabCam & getCamera();
			void setCursorEnabled(bool cursorEnabled=true);
			void setGridEnabled(bool gridEnabled);
			void setGridColor(const ofColor & gridColor);
			void setGridScale(const float gridScale);
			void setGridLabelsEnabled(bool ticksEnabled);

			
			void push(ofNode & node);
		protected:
			void drawPanel(const DrawArguments& arguments);
			vector<ofNode*> nodes;
			ofxGrabCam camera;
			ofColor gridColor;
			float gridScale;
			bool gridLabelsEnabled;
			bool gridEnabled;
		};
	}
}