#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Assets.h"

#include "ofNode.h"

//feel free to change this for a different camera type of your choosing
#include "ofxGrabCam.h"
typedef ofxGrabCam CameraType;

namespace ofxCvGui {
	namespace Panels {
		class Node : public Panels::Base {
		public:
			Node() { }
			Node(ofNode & node);
			
			//camera
			CameraType & getCamera() { return this->camera; };
			void setCursorEnabled(bool cursorEnabled=true) {
                this->camera.setCursorDraw(cursorEnabled);
            }
            
			void setGridEnabled(bool gridEnabled);
			void setGridColor(const ofColor & gridColor);
			void setGridScale(const float gridScale);
			void setGridLabelsEnabled(bool ticksEnabled);

			void push(ofNode & node);
		protected:
			void drawContent(DrawArguments& arguments);
			vector<ofNode*> nodes;
			CameraType camera;
			ofColor gridColor;
			float gridScale;
			bool gridLabelsEnabled;
			bool gridEnabled;
		};
	}
}