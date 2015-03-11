#include "SelectFile.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		SelectFile::SelectFile(string caption, function<string()> get, function<void(string &)> set) :
		LiveValue<string>(caption, get) {
			this->setFunction = set;
			//manually enable the edit button with our own actions
			// this might break if the code for LiveValue<T>::setEditable changes [2015-03-09]
			this->editButton->enable();
			this->editButton->onMouseReleased.addListener([this](MouseArguments &) {
				auto currentValue = this->liveValue();
				auto result = ofSystemLoadDialog(this->caption + " [" + currentValue + "]", false, currentValue);
				if (result.bSuccess) {
					this->setFunction(result.filePath);
				}
			}, this);
		}

		//----------
		SelectFile::SelectFile(ofParameter<string> & filename) :
			SelectFile(filename.getName(), [&filename]() { return filename; }, [&filename](string & newFilename) { filename = newFilename; }) {

		}
	}
}