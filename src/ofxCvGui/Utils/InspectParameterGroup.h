#pragma once

#include "../Element.h"
#include "../InspectController.h"

namespace ofxCvGui {
	namespace Utils {
		class InspectParameterGroup : public IInspectable {
		public:
			InspectParameterGroup(ofParameterGroup&);
			void inspect(InspectArguments&);
		protected:
			ofParameterGroup& parameters;
		};
	}

	// Utility function
	void inspectParameterGroup(ofParameterGroup&);
}