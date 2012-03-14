#pragma once
#include "ofxCvGui/Element.h"

namespace ofxCvGui {
	namespace Panels {
		class Base : public Element {
		public:
			virtual const ofPtr<Panels::Base> findScreen(const ofVec2f & xy) { return ofPtr<Panels::Base>(); }
		protected:
			virtual void drawPanel(const DrawArguments& arguments) = 0;
		private:
			void drawElement(const DrawArguments& arguments);
		};
	}
	typedef ofPtr<Panels::Base> PanelPtr;
}