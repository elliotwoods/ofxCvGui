#pragma once
#include "Scroll.h"

#include <set>

namespace ofxCvGui {
	namespace Panels {
		class Inspector : public Scroll {
		public:
			Inspector();
			~Inspector();

			void clear() override;

			bool getTitleEnabled() const;
			void setTitleEnabled(bool);
		protected:
			bool titleEnabled;
		};
	}
}