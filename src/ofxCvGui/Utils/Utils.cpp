#include "Utils.h"
#include <vector>
#include "ofAppRunner.h"
using namespace std;

namespace ofxCvGui {
	namespace Utils {
		//----------
		vector<ofRectangle> scissorHistory;
		
		//----------
		ofRectangle getScissor() {
			int bounds[4];
			glGetIntegeri_v(GL_SCISSOR_BOX, 0, bounds);
			return ofRectangle(bounds[0], bounds[1], bounds[2], bounds[3]);
		}

		//----------
		void applyScissor(const ofRectangle & bounds) {
			int x = (int) bounds.x;
			int y = (int) (ofGetWindowHeight() - (bounds.y + bounds.height));
			int width = (int) bounds.width;
			int height = (int) bounds.height;
			glEnable(GL_SCISSOR_TEST);
			glScissor(x, y, width, height);
		}

		//----------
		void pushScissor(const ofRectangle & bounds) {
			scissorHistory.push_back(getScissor());
			applyScissor(bounds);
		}

		//----------
		void popScissor() {
			applyScissor(scissorHistory.back());
			scissorHistory.pop_back();
			if(scissorHistory.empty()) {
				glDisable(GL_SCISSOR_TEST);
			}
		}
	}
}