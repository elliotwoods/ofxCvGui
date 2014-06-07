#pragma once

#include "../ElementGroup.h"
namespace ofxCvGui {
	namespace Widgets {
		class IInspectable {
		public:
			/// override this function to populate an inspector when selected
			virtual void populateInspector(ElementGroupPtr) = 0;
		};
	}
}