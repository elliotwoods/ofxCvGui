#pragma once
#include "Scroll.h"
#include "../Widgets/IInspectable.h"

#include <set>

namespace ofxCvGui {
	namespace Panels {
		class Inspector : public Scroll {
		public:
			Inspector();
			void clear();
			static void select(Widgets::IInspectable &);
			static void setSelection(Widgets::IInspectable &);
			static bool isSelected(Widgets::IInspectable &);
			static const set<Widgets::IInspectable *> getSelection();
			static ofxLiquidEvent<ElementGroupPtr> onClear;
		protected:
			bool initialised;
			static ofxLiquidEvent<Widgets::IInspectable> makeNewSelection;
			static ofxLiquidEvent<Widgets::IInspectable> makeNoSelection;

			static set<Widgets::IInspectable *> selection;
		};
	}
}