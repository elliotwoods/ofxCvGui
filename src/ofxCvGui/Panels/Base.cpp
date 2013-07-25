#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
        //---------
        Base::Base() {
			this->onDraw.addListener([this] (DrawArguments& args) {
				this->drawContent(args);
			}, this);
        }
        
        //---------
        Base::~Base() {
			this->onDraw.removeListeners(this);
        }
	}
}