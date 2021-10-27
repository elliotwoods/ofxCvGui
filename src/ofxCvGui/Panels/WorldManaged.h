#pragma once
#include "ofxCvGui/Panels/Base.h"

#include "ofNode.h"

#ifdef HAS_OFXGRABCAM
#define OFXCVGUI_USE_OFXGRABCAM
#endif

#ifdef OFXCVGUI_USE_OFXGRABCAM
#include "ofxGrabCam.h"
#else
#include "ofEasyCam.h"
#endif


namespace ofxCvGui {
	namespace Panels {
		class OFXCVGUI_API_ENTRY WorldManaged : public Panels::Base {
		public:

#ifdef OFXCVGUI_USE_OFXGRABCAM
			typedef ofxGrabCam CameraType;
#else
			typedef ofEasyCam CameraType;
#endif

			WorldManaged();

			//camera
			CameraType& getCamera();

			ofxLiquidEvent<ofCamera> onDrawWorld;

			struct Parameters : ofParameterGroup{
				struct : ofParameterGroup {
					ofParameter<bool> enabled{ "Enabled", true };
					ofParameter<bool> dark{ "Dark", true };
					ofParameter<glm::vec3> roomMin{ "Room min", {-1, -1, -1} };
					ofParameter<glm::vec3> roomMax{ "Room max", {1, 1, 1} };

					PARAM_DECLARE("Grid", enabled, dark, roomMin, roomMax);
				} grid;

				struct : ofParameterGroup {
					ofParameter<bool> enabled{ "Enabled", true };
					ofParameter<int> resolution{ "Resolution/x", 2 };
					ofParameter<float> blur{ "Blur", 1.5, 0, 4 };
					ofParameter<int> blurIterations{ "Blur iterations", 2};
					ofParameter<float> brightness{ "Brightness", 0.5f };
					ofParameter<bool> flipFloor{ "Flip floor", false };
					PARAM_DECLARE("Reflections", enabled, resolution, blur, blurIterations, brightness, flipFloor);
					ofEventListener resolutionListener;
				} reflections;

				ofParameter<bool> showCursor{ "Show cursor", true };

				PARAM_DECLARE("WorldManaged", grid, reflections, showCursor);
			} parameters;

		protected:
			void update();
			void drawContent(const ofRectangle& bounds);
			void drawGrid(bool forReflection);
			void showCursorCallback(bool&);

			void save();
			void load();

			bool cachedDark = false;
			CameraType camera;
			ofTexture gridTexture;

			ofFbo reflection[2]; // ping pong for blur
			bool needsReflectionAllocate = true;
		};

		shared_ptr<Panels::WorldManaged> makeWorldManaged(string caption = "");
	}
}