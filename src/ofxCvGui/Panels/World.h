#pragma once
#include "ofxCvGui/Panels/Base.h"

#include "ofNode.h"

//feel free to change this for a different camera type of your choosing
#include "../../../addons/ofxGrabCam/src/ofxGrabCam.h"
typedef ofxGrabCam CameraType;

namespace ofxCvGui {
	namespace Panels {
		class World : public Panels::Base {
		public:
			World();
			
			//camera
			CameraType & getCamera() { return this->camera; };
			void setCursorEnabled(bool cursorEnabled=true) {
                this->camera.setCursorDraw(cursorEnabled);
            }
            
			void setGridEnabled(bool gridEnabled);
			void setGridColor(const ofColor & gridColor);
			void setGridScale(const float gridScale);
			void setGridLabelsEnabled(bool ticksEnabled);

			ofxLiquidEvent<ofCamera> onDrawWorld;
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