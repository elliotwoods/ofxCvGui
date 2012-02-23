#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Assets.h"

namespace ofxCvGui {
	namespace Panels {
		class BaseImage : public Base {
		protected:
			void drawPanel(const DrawArguments& arguments);
			virtual void drawImage(const DrawArguments& arguments) = 0;
		};
	}
}