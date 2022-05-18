#pragma once
#include "../Element.h"
#include <string>

namespace ofxCvGui {
	namespace Widgets {
		class OFXCVGUI_API_ENTRY Heartbeat : public Element {
		public:
			typedef std::function<bool()> GetFunction;
			Heartbeat(const std::string & caption
				, const GetFunction &
				, float coolOffPeriod = 2.0f);
		protected:
			void init(std::string caption);
			void update();
			const GetFunction getFunction;
			float coolOffPeriod;
			std::chrono::system_clock::time_point lastHeartbeat;
			bool stateThisFrame = false;
		};
	}
}