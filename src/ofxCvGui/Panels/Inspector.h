#pragma once
#include "Scroll.h"
#include "../Widgets/IInspectable.h"

namespace ofxCvGui {
	namespace Panels {
		class Inspector : public Scroll {
		public:
			Inspector();
			void clear();
			static void select(Widgets::IInspectable &);

			static ofxLiquidEvent<ElementGroupPtr> onClear;
		protected:
			static ofxLiquidEvent<Widgets::IInspectable> onNewSelection;
		};
	}
}