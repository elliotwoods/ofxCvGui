#pragma once
#include "ofxCvGui/Element.h"
#include "ofxCvGui/Assets.h"

namespace ofxCvGui {
	namespace Panels {
		class Base : public Element {
		public:
			virtual const ofPtr<Panels::Base> findScreen(const ofVec2f & xy) { return ofPtr<Panels::Base>(); }
		protected:
			virtual void drawPanel(const DrawArguments& arguments) = 0;
			virtual void drawToolbar(float x) { }; ///< draws toolbar and returns width offset
		private:
			void drawElement(const DrawArguments& arguments);
		};
	}
	typedef ofPtr<Panels::Base> PanelPtr;
}