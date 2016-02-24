#pragma once

#include "LiveValue.h"

namespace ofxCvGui {
	namespace Widgets {
		template<typename Type>
		class EditableValue : public LiveValue<Type> {
		public:
			EditableValue(ofParameter<Type> & parameter) :
				LiveValue<Type>(parameter.getName(), [&parameter]() { return parameter;}) {
				this->setEditable(true);
				this->onEditValue += [&parameter, this](string & userValueString) {
					stringstream stream(userValueString);
					Type userValueTyped;
					stream >> userValueTyped;
					parameter.set(userValueTyped);
					
					this->onValueChange.notifyListeners(parameter.get());
				};
			}

			EditableValue(string name, Type & value) :
				LiveValue<Type>(name, [&value]() { return value; }) {
				this->setEditable(true);
				this->onEditValue += [&value, this](string & userValueString) {
					stringstream ss(userValueString);
					ss >> value;
					this->onValueChange.notifyListeners(value);
				};
			}

			EditableValue(string name, function<Type()> get, function<void(string)> set) :
				LiveValue<Type>(name, get) {
				this->setEditable(true);
				this->onEditValue += [set](string & userValueString) {
					set(userValueString);
				};
			}
			
			ofxLiquidEvent<const Type> onValueChange;
		};
		
		//specialisation for ofParameter<string>
		template<> EditableValue<string>::EditableValue(ofParameter<string> & parameter) :
		LiveValue<string>(parameter.getName(), [&parameter]() { return parameter;}) {
			this->setEditable(true);
			this->onEditValue += [&parameter, this](string & userValueString) {
				parameter.set(userValueString);
				this->onValueChange.notifyListeners(userValueString);
			};
		}
	}
}