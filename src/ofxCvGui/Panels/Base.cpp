#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
        //---------
        Base::Base() {
            ofAddListener(this->onDraw, this, &Base::drawContent);
        }
        
        //---------
        Base::~Base() {
            ofRemoveListener(this->onDraw, this, &Base::drawContent);
        }
	}
}