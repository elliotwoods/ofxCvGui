#pragma once
#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
		class BaseImage : public Base {
		public:
			void setAutoRefresh(bool autoRefresh);
			virtual void refresh() = 0;
		protected:
			BaseImage();
			void drawPanel(const DrawArguments& arguments);
			void drawToolbar(float x);
			virtual void drawImage(const DrawArguments& arguments) = 0;
			bool autoRefresh; ///<needs to become a button
		};
	}
}