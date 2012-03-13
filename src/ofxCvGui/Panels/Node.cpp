#include "ofxCvGui/Panels/Node.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Node::Node(ofNode & node) :
			node(node) {
		}

		//----------
		void Node::drawPanel(const DrawArguments& arguments) {
			this->camera.begin(this->getBounds());

			ofPushStyle();
			ofSetColor(200, 100, 100);
			ofDrawGrid(10.0f, 10.0f, true);
			ofPopStyle();
			
			this->node.draw();

			this->camera.end();
		}
	}
}