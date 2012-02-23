#pragma once
#include "ofxCvGui/Element.h"

namespace ofxCvGui {
	namespace Panels {
		class Base : public Element {
		protected:
			virtual void drawPanel(const DrawArguments& arguments) = 0;
		private:
			void drawElement(const DrawArguments& arguments);
		};
	}
	typedef ofPtr<Panels::Base> PanelPtr;
}