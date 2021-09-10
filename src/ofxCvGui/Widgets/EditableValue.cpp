#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		EditableValue<string>::EditableValue(ofParameter<string>& parameter) :
			LiveValue<string>(parameter.getName(), [&parameter]() { return parameter; })
		{
			this->setEditable(true);
			this->onEditValue += [&parameter, this](string& userValueString) {
				parameter.set(userValueString);
				this->onValueChange.notifyListeners(userValueString);
			};
		}

		//----------
		EditableValue<string>::EditableValue(string name, string& value) :
			LiveValue<string>(name, [&value]() { return value; }) {
			this->setEditable(true);
			this->onEditValue += [&value, this](string& userValueString) {
				stringstream ss(userValueString);
				ss >> value;
				this->onValueChange.notifyListeners(value);
			};
		}

		//----------
		EditableValue<string>::EditableValue(string name, function<string()> get, function<void(string)> set) :
			LiveValue<string>(name, get) {
			this->setEditable(true);
			this->onEditValue += [set](string& userValueString) {
				set(userValueString);
			};
		}
	}
}