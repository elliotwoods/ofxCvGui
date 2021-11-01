#include "pch_ofxCvGui.h"
#include "InspectParameterGroup.h"

namespace ofxCvGui {
	namespace Utils {
		//----------
		InspectParameterGroup::InspectParameterGroup(ofParameterGroup& parameters)
			: parameters(parameters) {
			this->onPopulateInspector += [this](InspectArguments& args) {
				this->inspect(args);
			};
		}

		//----------
		void InspectParameterGroup::inspect(InspectArguments& args) {
			auto inspector = args.inspector;
			inspector->addParameterGroup(this->parameters);
		}
	}

	//----------
	void inspectParameterGroup(ofParameterGroup& parameters) {
		auto inspectable = make_shared<Utils::InspectParameterGroup>(parameters);
		ofxCvGui::inspectWithOwnership(inspectable);
	}
}