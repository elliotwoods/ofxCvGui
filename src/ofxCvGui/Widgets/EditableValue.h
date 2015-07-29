#pragma once

#include "LiveValue.h"

namespace ofxCvGui {
	namespace Widgets {
		template<typename Type>
		class EditableValue : public LiveValue<Type> {
		public:
			OFXCVGUI_MAKE_ELEMENT_HEADER(EditableValue<Type>, ofParameter<Type> & parameter) {
				OFXCVGUI_MAKE_ELEMENT_BODY(EditableValue<Type>, parameter);
			}

			OFXCVGUI_MAKE_ELEMENT_HEADER(EditableValue<Type>, string caption, Type & value) {
				OFXCVGUI_MAKE_ELEMENT_BODY(EditableValue<Type>, caption, value);
			}

			OFXCVGUI_MAKE_ELEMENT_HEADER(EditableValue<Type>, string caption, function<Type()> get, function<void(string)> set) {
				OFXCVGUI_MAKE_ELEMENT_BODY(EditableValue<Type>, caption, get, set);
			}

			EditableValue(ofParameter<Type> & parameter) : 
				LiveValue<Type>(parameter.getName(), [&parameter]() {	return parameter;}) {
				this->setEditable(true);
				this->onEditValue += [&parameter](string & userValueString) {
					stringstream stream(userValueString);
					Type userValueTyped;
					stream >> userValueTyped;
					parameter.set(userValueTyped);
				};
			}

			EditableValue(string name, Type & value) :
				LiveValue<Type>(name, [&value]() { return value; }) {
				this->setEditable(true);
				this->onEditValue += [&value](string & userValueString) {
					stringstream ss(userValueString);
					ss >> value;
				};
			}

			EditableValue(string name, function<Type()> get, function<void(string)> set) :
				LiveValue<Type>(name, get) {
				this->setEditable(true);
				this->onEditValue += [set](string & userValueString) {
					set(userValueString);
				};
			}
		};
	}
}