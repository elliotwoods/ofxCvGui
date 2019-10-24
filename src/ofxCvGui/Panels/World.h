#pragma once
#include "ofxCvGui/Panels/Base.h"

#include "ofNode.h"

#ifdef HAS_OFXGRABCAM
    #define OFXCVGUI_USE_OFXGRABCAM
#endif

#define OFXCVGUI_USE_OFXGRABCAM

#ifdef OFXCVGUI_USE_OFXGRABCAM
	//feel free to change this for a different camera type of your choosing
	#include "ofxGrabCam.h"
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
			CameraType & getCamera();
#ifdef OFXCVGUI_USE_OFXGRABCAM
			void setCursorEnabled(bool cursorEnabled = true);
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

		shared_ptr<Panels::World> makeWorld(string caption = "");
	}
}
