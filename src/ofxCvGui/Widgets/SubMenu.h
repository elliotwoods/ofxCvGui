#pragma once

#include "Button.h"

namespace ofxCvGui {
	class IInspectable;
	class InspectArguments;

	namespace Widgets {
		class OFXCVGUI_API_ENTRY SubMenuInspectable : public Button {
		public:
			SubMenuInspectable(const std::string& caption
				, std::shared_ptr<IInspectable>
				, char hotKey = 0);
		};

		class OFXCVGUI_API_ENTRY SubMenuFunctional : public SubMenuInspectable {
		public:
			SubMenuFunctional(const std::string& caption
				, const std::function<void(InspectArguments&)>&
				, char hotKey = 0);
		};
	}
}
