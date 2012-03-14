#include "ofxCvGui/Panels/Node.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Node::Node(ofNode & node) {
			this->push(node);
		}

		//----------
		void Node::drawPanel(const DrawArguments& arguments) {
			this->camera.begin(this->getBounds());

			ofPushStyle();
			ofSetColor(200, 100, 100);
			ofDrawGrid(10.0f, 10.0f, true);
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
		void Node::push(ofNode & node) {
			this->nodes.push_back(&node);
		}
	}
}