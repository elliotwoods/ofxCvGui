#pragma once
#include "ofxCvGui/Panels/Base.h"
//WARNING!!!
//this may be depreciated after widgets are introduced

namespace ofxCvGui {
	namespace Panels {
		class Text : public Panels::Base {
		public:
			Text(string text = "");
            virtual ~Text();
			
			string & getText() { return this->text; }
			
		protected:
			void drawText(DrawArguments& arguments);
			string text;
		};
	}
}