#include "pch_ofxCvGui.h"
#include "WorldManaged.h"

namespace ofxCvGui {
	namespace Panels {
		//---------
		WorldManaged::WorldManaged()
		{
			auto wasArbTex = ofGetUsingArbTex();
			ofDisableArbTex();
			{
				this->gridTexture.enableMipmap();
				this->gridTexture.loadData(ofxAssets::image("ofxRulr::grid-10").getPixels());
				this->gridTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);
				this->gridTexture.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
			}
			if (wasArbTex) ofEnableArbTex();

#ifdef OFXCVGUI_USE_OFXGRABCAM
			this->parameters.showCursor.addListener(this, &WorldManaged::showCursorCallback);
			this->camera.setCursorDrawEnabled(this->parameters.showCursor.get());
#endif
#ifdef OFXCVGUI_USE_OFXGRABCAM
			this->onUpdate += [this](UpdateArguments& args) {
				ofEventArgs dummyArgs;
				this->camera.update(dummyArgs);
			};
			this->camera.setFixUpDirectionEnabled(true);
			this->camera.setListenersEnabled(false);
#endif

			this->onDraw += [this](DrawArguments& args) {
				this->drawContent(args.globalBounds);
			};

#ifdef OFXCVGUI_USE_OFXGRABCAM
			this->onMouse += [this](MouseArguments& args) {
				args.takeMousePress(this);

				ofMouseEventArgs ofArgs;

				ofArgs.button = args.button;

				switch (args.action) {
				case MouseArguments::Action::Moved:
					ofArgs.type = ofMouseEventArgs::Type::Moved;
					this->camera.mouseMoved(ofArgs);
					break;
				case MouseArguments::Action::Pressed:
					ofArgs.type = ofMouseEventArgs::Type::Pressed;
					this->camera.mousePressed(ofArgs);
					break;
				case MouseArguments::Action::Released:
					ofArgs.type = ofMouseEventArgs::Type::Released;
					this->camera.mouseReleased(ofArgs);
					break;
				case MouseArguments::Action::Dragged:
					ofArgs.type = ofMouseEventArgs::Type::Dragged;
					this->camera.mouseDragged(ofArgs);
					break;
				default:
					break;
				}
			};

			this->onKeyboard += [this](KeyboardArguments& args) {
				ofKeyEventArgs ofArgs;
				ofArgs.key = args.key;
				if (args.action == KeyboardArguments::Action::Pressed) {
					ofArgs.type = ofKeyEventArgs::Type::Pressed;
					this->camera.keyPressed(ofArgs);
				}
				else if (args.action == KeyboardArguments::Action::Released) {
					ofArgs.type = ofKeyEventArgs::Type::Released;
					this->camera.keyReleased(ofArgs);
				}
			};
#endif
		}

		shared_ptr<Panels::WorldManaged> makeWorldManaged(string caption) {
			auto newPanel = shared_ptr<Panels::WorldManaged>(new Panels::WorldManaged());
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}

		//---------
		WorldManaged::CameraType&
			WorldManaged::getCamera()
		{
			return this->camera;
		}

		//----------
		void
			WorldManaged::drawContent(const ofRectangle& bounds)
		{
			this->camera.begin(bounds);
			{
				if (this->parameters.grid.enabled) {
					this->drawGrid();
				}

				this->onDrawWorld.notifyListeners(this->camera);

			}
			this->camera.end();

			// Draw annotations on top of 3D view
			Utils::AnnotationManager::X().renderAndClearAnnotations(this->camera, bounds);
		}

		//----------
		void
			WorldManaged::drawGrid()
		{
			const auto& roomMinimum = this->parameters.grid.roomMin.get();
			const auto& roomMaximum = this->parameters.grid.roomMax.get();
			const auto roomSpan = roomMaximum - roomMinimum;

			//--
			//axes
			//--
			//
			ofPushStyle();
			ofSetLineWidth(2.0f);
			//
			//x
			ofSetColor(200, 100, 100);
			ofPushMatrix();
			ofTranslate(0, roomMaximum.y, roomMaximum.z);
			ofDrawLine(roomMinimum.x, 0, roomMaximum.x, 0);
			ofPopMatrix();
			//
			//y
			ofSetColor(100, 200, 100);
			ofDrawLine(0, roomMaximum.y, roomMaximum.z, 0, roomMinimum.y, roomMaximum.z);
			//
			//z
			ofSetColor(100, 200, 100);
			ofDrawLine(0, roomMaximum.y, roomMaximum.z, 0, roomMaximum.y, roomMinimum.z);
			//
			ofPopStyle();
			//
			//--



#ifdef OFXCVGUI_USE_OFXGRABCAM
			//--
			//cursor lines
			//--
			//
			ofPushStyle();
			{
				const auto cursorPosition = camera.getCursorWorld();
				//
				if (this->parameters.grid.dark) {
					ofSetColor(100);
				}
				else {
					ofSetColor(0);
				}
				ofSetLineWidth(2.0f);
				//front wall
				ofDrawLine(cursorPosition.x, roomMaximum.y, roomMinimum.z, cursorPosition.x, roomMinimum.y, roomMinimum.z); //x
				ofDrawLine(roomMinimum.x, cursorPosition.y, roomMinimum.z, roomMaximum.x, cursorPosition.y, roomMinimum.z); //y
				//back wall
				ofDrawLine(cursorPosition.x, roomMaximum.y, roomMaximum.z, cursorPosition.x, roomMinimum.y, roomMaximum.z); //x
				ofDrawLine(roomMinimum.x, cursorPosition.y, roomMaximum.z, roomMaximum.x, cursorPosition.y, roomMaximum.z); //y
				//
				//floor
				ofDrawLine(cursorPosition.x, roomMaximum.y, roomMinimum.z, cursorPosition.x, roomMaximum.y, roomMaximum.z); //x
				ofDrawLine(roomMinimum.x, roomMaximum.y, cursorPosition.z, roomMaximum.x, roomMaximum.y, cursorPosition.z); //z
				//ceiling
				ofDrawLine(cursorPosition.x, roomMinimum.y, roomMinimum.z, cursorPosition.x, roomMinimum.y, roomMaximum.z); //x
				ofDrawLine(roomMinimum.x, roomMinimum.y, cursorPosition.z, roomMaximum.x, roomMinimum.y, cursorPosition.z); //z
				//
				//left wall
				ofDrawLine(roomMinimum.x, cursorPosition.y, roomMinimum.z, roomMinimum.x, cursorPosition.y, roomMaximum.z); //y
				ofDrawLine(roomMinimum.x, roomMinimum.y, cursorPosition.z, roomMinimum.x, roomMaximum.y, cursorPosition.z); //z
				//right wall
				ofDrawLine(roomMaximum.x, cursorPosition.y, roomMinimum.z, roomMaximum.x, cursorPosition.y, roomMaximum.z); //y
				ofDrawLine(roomMaximum.x, roomMinimum.y, cursorPosition.z, roomMaximum.x, roomMaximum.y, cursorPosition.z); //z
				//
			}
			ofPopStyle();
			//
			//--
#endif



			//--
			//grid
			//--
			//
			glEnable(GL_CULL_FACE);
			this->gridTexture.bind();
			{
				//
				//front/back walls
				ofPushMatrix();
				{
					auto planeXY = ofPlanePrimitive(roomSpan.x, roomSpan.y, 2, 2);
					planeXY.mapTexCoords(roomMinimum.x, roomSpan.y, roomMaximum.x, 0);

					//front
					glCullFace(GL_FRONT);
					ofTranslate(roomMinimum.x + roomSpan.x * 0.5, roomMaximum.y - roomSpan.y * 0.5, roomMinimum.z);
					planeXY.draw();

					//back
					glCullFace(GL_BACK);
					ofTranslate(0, 0, roomSpan.z);
					planeXY.draw();

					ofDisableLighting();
				}
				ofPopMatrix();
				//
				//floor/ceiling
				ofPushMatrix();
				{
					auto planeXZ = ofPlanePrimitive(roomSpan.x, roomSpan.z, 2, 2);
					planeXZ.mapTexCoords(roomMinimum.x, roomSpan.z, roomMaximum.x, 0);

					ofTranslate(roomMinimum.x + roomSpan.x * 0.5, roomMaximum.y, 0);
					ofRotate(90, -1, 0, 0);

					//floor
					glCullFace(GL_BACK);
					ofTranslate(0, roomSpan.z * 0.5 - roomMaximum.z, 0);
					planeXZ.draw();

					//ceiling
					glCullFace(GL_FRONT);
					ofTranslate(0, 0, -roomSpan.y);
					planeXZ.draw();
				}
				ofPopMatrix();
				//
				//side walls
				ofPushMatrix();
				{
					auto planeYZ = ofPlanePrimitive(roomSpan.z, roomSpan.y, 2, 2);
					planeYZ.mapTexCoords(roomMinimum.z, roomSpan.y, roomMaximum.z, 0);

					ofTranslate(0, roomMaximum.y - roomSpan.y * 0.5, roomMaximum.z - roomSpan.z * 0.5);
					ofRotate(-90, 0, 1, 0);

					//left wall
					glCullFace(GL_BACK);
					ofTranslate(0, 0, -roomMinimum.x);
					planeYZ.draw();

					//right wall
					glCullFace(GL_FRONT);
					ofTranslate(0, 0, -roomSpan.x);
					planeYZ.draw();
				}
				ofPopMatrix();
				//
			}
			this->gridTexture.unbind();
			glDisable(GL_CULL_FACE);
			//
			//--
		}

		//----------
		void WorldManaged::showCursorCallback(bool & cursorEnabled)
		{
#ifdef OFXCVGUI_USE_OFXGRABCAM
			this->camera.setCursorDrawEnabled(cursorEnabled);
#endif
		}
	}
}