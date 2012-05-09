#pragma once
#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "types/ofBaseTypes.h"

namespace ofxCvGui {
	namespace Panels {
		class Draws : public Panels::Base {
		public:
			Draws(ofBaseDraws& pixels);
		protected:
			void drawPanel(const DrawArguments& arguments);
		private:
			ofBaseDraws& draws;
		};
	}
}