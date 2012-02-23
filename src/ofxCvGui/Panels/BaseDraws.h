#pragma once
#pragma once
#include "ofxCvGui/Element.h"
#include "ofBaseTypes.h"

namespace ofxCvGui {
	namespace Panels {
		class BaseDraws : public Element {
		protected:
			BaseDraws(ofBaseDraws& pixels);
			void customDraw(const DrawArguments& arguments);
		private:
			ofBaseDraws& draws;
		};
	}
}