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
				struct Grid : ofParameterGroup {
					ofParameter<bool> enabled{ "Enabled", true };
					ofParameter<bool> dark{ "Dark", false };
					ofParameter<glm::vec3> roomMin{ "Room min", {-1, -1, -1} };
					ofParameter<glm::vec3> roomMax{ "Room max", {1, 1, 1} };

					Grid() {
						this->setName("Grid");
						this->add(enabled, dark, roomMin, roomMax);
					}
				} grid;

				ofParameter<bool> showCursor;

				Parameters() {
					this->setName("WorldManaged");
					this->add(grid, showCursor);
				}
			} parameters;

		protected:
			void drawContent(const ofRectangle& bounds);
			void drawGrid();
			void showCursorCallback(bool&);

			CameraType camera;
			ofTexture gridTexture;
		};

		shared_ptr<Panels::WorldManaged> makeWorldManaged(string caption = "");
	}
}