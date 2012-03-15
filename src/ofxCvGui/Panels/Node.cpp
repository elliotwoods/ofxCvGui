#include "ofxCvGui/Panels/Node.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Node::Node(ofNode & node) {
			this->push(node);
			this->gridColor = ofColor(60, 50, 50);
		}

		//----------
		void Node::drawPanel(const DrawArguments& arguments) {
			this->camera.begin(this->getBounds());

			ofPushStyle();
			ofSetColor(this->gridColor);
			ofDrawGrid(10.0f, 10.0f, this->gridLabelsEnabled);
			ofPopStyle();

			vector<ofNode*>::iterator it;
			for (it = this->nodes.begin(); it != this->nodes.end(); it++)
				(**it).draw();

			this->camera.end();
		}
		
		//----------
		void Node::setCursorEnabled(bool cursorEnabled) {
			this->camera.setCursorDraw(cursorEnabled);
		}

		//----------
		void Node::setGridColor(const ofColor & gridColor) {
			this->gridColor = gridColor;
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