#pragma once
#include "Widgets.h"

#include <set>

namespace ofxCvGui {
	namespace Panels {
		class OFXCVGUI_API_ENTRY Inspector : public Widgets, public enable_shared_from_this<Inspector> {
		public:
			Inspector();
			~Inspector();

			void clear(bool notifyListeners = true);

			bool getTitleEnabled() const;
			void setTitleEnabled(bool);
		protected:
			bool titleEnabled;
		};

		shared_ptr<Panels::Inspector> makeInspector();
	}
}