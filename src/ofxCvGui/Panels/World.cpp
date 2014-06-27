#include "ofxCvGui/Panels/World.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		World::World() {
			this->gridColor = ofColor(90, 50, 50);
			this->gridEnabled = true;
			this->gridLabelsEnabled = true;
			this->gridScale = 10.0f;

			this->onDraw += [this] (DrawArguments & args) {
				this->drawContent(args);
			};
		}

		//----------
		void World::drawContent(DrawArguments& arguments) {
			this->camera.begin(arguments.globalBounds);

			if (this->gridEnabled) {
				ofPushStyle();
				ofSetColor(this->gridColor);
				ofDrawGrid(gridScale, 5.0f, this->gridLabelsEnabled);
				ofPopStyle();
			}

			vector<ofNode*>::iterator it;
			for (it = this->nodes.begin(); it != this->nodes.end(); it++)
				(**it).draw();
			this->onDrawWorld.notifyListeners(this->camera);

			this->camera.end();
		}

		//----------
		void World::setGridEnabled(bool gridEnabled) {
			this->gridEnabled = gridEnabled;
		}
		
		//----------
		void World::setGridColor(const ofColor & gridColor) {
			this->gridColor = gridColor;
		}
		
		//----------
		void World::setGridScale(const float gridScale) {
			this->gridScale = gridScale;
		}

		//----------
		void World::setGridLabelsEnabled(bool gridLabelsEnabled) {
			this->gridLabelsEnabled = gridLabelsEnabled;
		}
	}
}