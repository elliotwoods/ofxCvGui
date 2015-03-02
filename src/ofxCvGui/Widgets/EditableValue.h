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

			EditableValue(ofParameter<Type> & parameter) : 
				LiveValue(parameter.getName(), [&parameter]() {	return parameter;}) {
				this->setEditable(true);
				this->onEditValue += [&parameter](string & userValueString) {
					stringstream stream(userValueString);
					Type userValueTyped;
					stream >> userValueTyped;
					parameter.set(userValueTyped);
				};
			}
		};
	}
}