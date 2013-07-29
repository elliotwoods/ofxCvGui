#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Assets.h"

#include "ofNode.h"

#ifdef HAS_ADDON_OFXGRABCAM
    #include "ofxGrabCam.h"
    typedef ofxGrabCam CameraType;
#else
    #include "ofEasyCam.h"
    typedef ofEasyCam CameraType;
#endif

namespace ofxCvGui {
	namespace Panels {
		class Node : public Panels::Base {
		public:
			Node();
			Node(ofNode & node);
			virtual ~Node();
			
			//camera
			CameraType & getCamera() { return this->camera; };
			void setCursorEnabled(bool cursorEnabled=true) {
#ifdef HAS_ADDON_OFXGRABCAM
                this->camera.setCursorDraw(cursorEnabled);
#endif
            }
            
			void setGridEnabled(bool gridEnabled);
			void setGridColor(const ofColor & gridColor);
			void setGridScale(const float gridScale);
			void setGridLabelsEnabled(bool ticksEnabled);

			void push(ofNode & node);
		protected:
			void drawNodes(DrawArguments& arguments);
			vector<ofNode*> nodes;
			CameraType camera;
			ofColor gridColor;
			float gridScale;
			bool gridLabelsEnabled;
			bool gridEnabled;
		};
	}
}