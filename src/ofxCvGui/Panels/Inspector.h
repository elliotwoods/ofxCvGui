#pragma once
#include "Scroll.h"
#include "../Widgets/IInspectable.h"

#include <set>

namespace ofxCvGui {
	namespace Panels {
		class Inspector : public Scroll {
		public:
			Inspector();
			~Inspector();
			void clear();
			static void select(Widgets::IInspectable &);
			static void setSelection(Widgets::IInspectable &);
			static bool isSelected(Widgets::IInspectable &);
			static const set<Widgets::IInspectable *> getSelection();
			static void addWidget(ElementPtr);
			static void refresh();
			static ofxLiquidEvent<ElementGroupPtr> onClear;
		protected:
			static ofxLiquidEvent<Widgets::IInspectable> makeNewSelection;
			static ofxLiquidEvent<Widgets::IInspectable> makeNoSelection;
			static ofxLiquidEvent<ElementPtr> addWidgetEvent;

			static set<Widgets::IInspectable *> selection;
		};
	}
}