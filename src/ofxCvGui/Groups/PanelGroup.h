#pragma once
#include "ofxCvGui/ElementGroup.h"
#include "ofMath.h"
namespace ofxCvGui {
	namespace Groups {
		class PanelGroup : public ElementGroup {
		protected:
			void boundsChange();
		};
	}
}