#pragma once

#include "EditableValue.h"

#include "ofParameter.h"
#include "ofSystemUtils.h"

namespace ofxCvGui {
	namespace Widgets {
		class SelectFile : public LiveValue<string> {
		public:
			SelectFile(string caption, function<string()> get, function<void(string &)> set);
			SelectFile(ofParameter<string> &);
		protected:
			function<void(string &)> setFunction;
		};
	}
}