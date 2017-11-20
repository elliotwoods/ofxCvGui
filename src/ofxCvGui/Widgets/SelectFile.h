#pragma once

#include "EditableValue.h"

#include "ofParameter.h"
#include "ofSystemUtils.h"

namespace ofxCvGui {
	namespace Widgets {
		class SelectFile : public LiveValue<filesystem::path> {
		public:
			SelectFile(string caption, function<filesystem::path()> get, function<void(filesystem::path &)> set);
			SelectFile(ofParameter<filesystem::path> &);
			ofxLiquidEvent<const filesystem::path &> onValueChange;
		protected:
			function<void(filesystem::path &)> setFunction;
		};
	}
}