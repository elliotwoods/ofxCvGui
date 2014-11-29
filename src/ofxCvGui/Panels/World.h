#pragma once
#include "ofxCvGui/Panels/Base.h"

#include "ofNode.h"

#define USE_OFXGRABCAM

#ifdef USE_OFXGRABCAM
	//feel free to change this for a different camera type of your choosing
	#include "../../../addons/ofxGrabCam/src/ofxGrabCam.h"
	typedef ofxGrabCam CameraType;
#else
	#include "ofEasyCam.h"
	typedef ofEasyCam CameraType;
#endif

namespace ofxCvGui {
	namespace Panels {
		class World : public Panels::Base {
		public:
			World();
			
			//camera
			CameraType & getCamera() { return this->camera; };
#ifdef USE_OFXGRABCAM
			void setCursorEnabled(bool cursorEnabled=true) {
                this->camera.setCursorDraw(cursorEnabled);
            }
#endif
            
			void setGridEnabled(bool gridEnabled);
			void setGridColor(const ofColor & gridColor);
			void setGridScale(const float gridScale);
			void setGridLabelsEnabled(bool ticksEnabled);

			void setUseFbo(bool);
			bool getUseFbo() const;

			ofxLiquidEvent<ofCamera> onDrawWorld;
		protected:
			void drawContent(const ofRectangle & bounds);
			void allocateFbo();

			vector<ofNode*> nodes;
			CameraType camera;
			ofColor gridColor;
			float gridScale;
			bool gridLabelsEnabled;
			bool gridEnabled;

			bool useFbo;
			ofFbo fbo;
		};
	}
}