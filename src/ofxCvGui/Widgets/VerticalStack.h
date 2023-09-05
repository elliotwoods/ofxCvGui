#pragma once
#include "../ElementGroup.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class OFXCVGUI_API_ENTRY VerticalStack : public ElementGroup {
		public:
			enum Layout {
				DistributeEvenly
				, UseElementHeight
			};

			VerticalStack(const Layout& layout = Layout::DistributeEvenly);
		protected:
			bool needsNewWidth = false;
			Layout layout;
		};
	}
}