#include "ofxCvGui/Panels/Node.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Node::Node() {
			this->onDraw.addListener([this] (DrawArguments& args) {
				this->drawNodes(args);
				if (args.chromeEnabled) {
					AssetRegister.drawText(this->caption, 20, 20, "", true, 30);
				}
			}, this);

			this->gridScale = 5.0f;
		}

		//----------
		Node::~Node() {
			this->onDraw.removeListeners(this);
		}

		//----------
		void Node::drawNodes(DrawArguments& arguments) {
			this->camera.begin(this->getBounds());

			if (this->gridEnabled) {
				ofPushStyle();
				ofSetColor(this->gridColor);
				ofDrawGrid(gridScale, 5.0f, this->gridLabelsEnabled);
				ofPopStyle();
			}

			vector<ofNode*>::iterator it;
			for (it = this->nodes.begin(); it != this->nodes.end(); it++)
				(**it).draw();

			this->onDrawWorld(this->getCamera());

			this->camera.end();
		}

		//----------
		void Node::setGridEnabled(bool gridEnabled) {
			this->gridEnabled = gridEnabled;
		}
		
		//----------
		void Node::setGridColor(const ofColor & gridColor) {
			this->gridColor = gridColor;
		}
		
		//----------
		void Node::setGridScale(const float gridScale) {
			this->gridScale = gridScale;
		}

		//----------
		void Node::setGridLabelsEnabled(bool gridLabelsEnabled) {
			this->gridLabelsEnabled = gridLabelsEnabled;
		}

		//----------
		void Node::push(ofNode & node) {
			this->nodes.push_back(&node);
		}
	}
}