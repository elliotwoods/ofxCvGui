#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Assets.h"

#include "ofNode.h"
#include "ofxGrabCam.h"

namespace ofxCvGui {
	namespace Panels {
		class Node : public Panels::Base {
		public:
			Node(ofNode & node);
		protected:
			void drawPanel(const DrawArguments& arguments);
			ofNode & node;
			ofxGrabCam camera;
		};
	}
}