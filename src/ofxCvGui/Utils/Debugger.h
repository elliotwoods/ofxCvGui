#pragma once

#include "ofxSingleton.h"
#include "ofParameterGroup.h"
#include "Sugar.h"

namespace ofxCvGui {
	namespace Utils {
		class Debugger : public ofxSingleton::Singleton<Debugger> {
		public:
			struct : ofParameterGroup {
				struct : ofParameterGroup {
					ofParameter<bool> timings{ "Timings", false };
					PARAM_DECLARE("Draw", timings);
				} draw;
				PARAM_DECLARE("Debugger", draw);
			} parameters;
		};
	}
}