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
				, bool ownInspectable = false
				, char hotKey = 0);
		};

		class OFXCVGUI_API_ENTRY SubMenuFunctional : public SubMenuInspectable {
		public:
			SubMenuFunctional(const std::string& caption
				, const std::function<void(InspectArguments&)>&
				, bool ownInspectable = false
				, char hotKey = 0);
		};
	}
}
