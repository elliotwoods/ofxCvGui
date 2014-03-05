#pragma once

#include "../ElementGroup.h"
namespace ofxCvGui {
	namespace Widgets {
		class IInspectable {
		public:
			/// override this function to populate an inspector when selected
			virtual void populate(ElementGroupPtr) = 0;
		};
	}
}