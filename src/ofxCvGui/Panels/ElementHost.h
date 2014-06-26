#pragma once
#include "Base.h"
#include "ofxCvGui/ElementGroup.h"

namespace ofxCvGui {
	namespace Panels {
		class ElementHost : public Base {
		public:
			ElementHost();
			ElementGroupPtr getElementGroup();
		protected:
			ElementGroupPtr elementGroup;
		};
	}
}