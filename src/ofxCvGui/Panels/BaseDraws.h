#pragma once
#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "types/ofBaseTypes.h"

namespace ofxCvGui {
	namespace Panels {
		class BaseDraws : public Panels::Base {
		public:
			BaseDraws(ofBaseDraws& pixels);
		protected:
			void customDraw(const DrawArguments& arguments);
		private:
			ofBaseDraws& draws;
		};
	}
}