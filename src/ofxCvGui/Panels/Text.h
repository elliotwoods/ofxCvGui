#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxCvGui/Assets.h"
//WARNING!!!
//this may be depreciated after widgets are introduced

namespace ofxCvGui {
	namespace Panels {
		class Text : public Panels::Base {
		public:
			Text(string text = "");
		protected:
			void drawPanel(const DrawArguments& arguments);
			string text;
		};
	}
}