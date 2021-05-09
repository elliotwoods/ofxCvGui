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
				this->gridTexture.loadData(ofxAssets::image("ofxCvGui::grid-10").getPixels());
				this->gridTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);
				this->gridTexture.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
			}
			if (wasArbTex) ofEnableArbTex();

#ifdef OFXCVGUI_USE_OFXGRABCAM
			this->parameters.showCursor.addListener(this, &WorldManaged::showCursorCallback);
			this->camera.setCursorDrawEnabled(this->parameters.showCursor.get());
		
			this->onUpdate += [this](UpdateArguments& args) {
				this->update();
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
				(glm::vec2&)ofArgs = args.global;

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

			this->onBoundsChange += [this](BoundsChangeArguments&) {
				this->needsReflectionAllocate = true;
			};
			this->parameters.reflections.resolutionListener = this->parameters.reflections.resolution.newListener([this](const int&) {
				this->needsReflectionAllocate = true;
				});
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

		//---------
		void
			WorldManaged::update()
		{
			ofEventArgs dummyArgs;
			this->camera.update(dummyArgs);

			if (!this->cachedDark && this->parameters.grid.dark.get()) {
				this->gridTexture.loadData(ofxAssets::image("ofxCvGui::grid-10-dark"));
				this->cachedDark = true;
			}
			else if (this->cachedDark && !this->parameters.grid.dark.get()) {
				this->gridTexture.loadData(ofxAssets::image("ofxCvGui::grid-10"));
				this->cachedDark = false;
			}
		}

		//----------
		void
			WorldManaged::drawContent(const ofRectangle& bounds)
		{
			auto& roomMin = this->parameters.grid.roomMin.get();
			auto& roomMax = this->parameters.grid.roomMax.get();

			if (this->parameters.reflections.enabled) {
				if (this->needsReflectionAllocate) {
					ofFbo::Settings settings;
					{
						settings.width = this->getWidth() / this->parameters.reflections.resolution;
						settings.height = this->getHeight() / this->parameters.reflections.resolution;
						settings.numSamples = 4;
						settings.internalformat = GL_RGBA;
					}

					for (int i = 0; i < 2; i++) {
						this->reflection[i].allocate(settings);
					}
					this->needsReflectionAllocate = false;
				}

				// Render into the buffer
				this->reflection[0].begin(ofFboMode::OF_FBOMODE_NODEFAULTS);
				{
					ofClear(0, 255);
					Utils::AnnotationManager::X().setEnabled(false);
					ofPushView();
					{
						ofSetMatrixMode(ofMatrixMode::OF_MATRIX_PROJECTION);
						ofLoadMatrix(this->camera.getProjectionMatrix());
						ofSetMatrixMode(ofMatrixMode::OF_MATRIX_MODELVIEW);
						ofLoadMatrix(this->camera.getModelViewMatrix());

						ofScale(1, -1, 1);
						ofTranslate(0,  -roomMin.y * 2, 0);
						this->onDrawWorld.notifyListeners();
					}
					ofPopView();
					Utils::AnnotationManager::X().setEnabled(true);
				}
				this->reflection[0].end();

				// Blur the buffer
				for (int i = 0; i < this->parameters.reflections.blurIterations.get(); i++) {
					this->reflection[1].begin();
					{
						auto& texture = this->reflection[0].getTexture();

						ofClear(0, 0);
						auto shader = ofxAssets::shader("ofxCvGui::blurX");
						shader.begin();
						{
							shader.setUniform1f("blurAmnt", this->parameters.reflections.blur.get());
							texture.draw(0, 0);
						}
						shader.end();
					}
					this->reflection[1].end();

					this->reflection[0].begin();
					{
						ofClear(0, 0);
						auto shader = ofxAssets::shader("ofxCvGui::blurX");
						shader.begin();
						{
							shader.setUniform1f("blurAmnt", this->parameters.reflections.blur.get());
							this->reflection[0].draw(0, 0);
						}
						shader.end();
					}
					this->reflection[0].end();
				}
				
			}

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
					ofRotateDeg(90, -1, 0, 0);

					//ceiling
					glCullFace(GL_BACK);
					ofTranslate(0, roomSpan.z * 0.5 - roomMaximum.z, 0);
					planeXZ.draw();

					//floor
					glCullFace(GL_FRONT);
					ofTranslate(0, 0, -roomSpan.y);
					if (this->parameters.reflections.enabled) {
						auto& shader = ofxAssets::shader("ofxCvGui::reflection");
						auto& reflectionTexture = this->reflection[0].getTexture();
						shader.begin();
						{
							shader.setUniform2f("resolution", { this->getWidth(), this->getHeight() });
							shader.setUniform1f("resolutionDivider", this->parameters.reflections.resolution.get());
							shader.setUniform1f("reflectionBrightness", this->parameters.reflections.brightness.get());
							shader.setUniformTexture("reflection", reflectionTexture, 1);
							planeXZ.draw();
						}
						shader.end();
					}
					else {
						planeXZ.draw();
					}
				}
				ofPopMatrix();
				//
				//side walls
				ofPushMatrix();
				{
					auto planeYZ = ofPlanePrimitive(roomSpan.z, roomSpan.y, 2, 2);
					planeYZ.mapTexCoords(roomMinimum.z, roomSpan.y, roomMaximum.z, 0);

					ofTranslate(0, roomMaximum.y - roomSpan.y * 0.5, roomMaximum.z - roomSpan.z * 0.5);
					ofRotateDeg(-90, 0, 1, 0);

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