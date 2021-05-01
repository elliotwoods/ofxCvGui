#include "pch_ofxCvGui.h"

ostream& operator<<(ostream& stream, const ofxCvGui::BaseEnum& value) {
	stream << value.toString();
	return stream;
}

istream& operator>>(istream& stream, ofxCvGui::BaseEnum& value) {
	string valueString;
	stream >> valueString;
	value.fromString(valueString);
	return stream;
}