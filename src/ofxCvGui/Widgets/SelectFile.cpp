#include "SelectFile.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		SelectFile::SelectFile(string caption, function<filesystem::path()> get, function<void(filesystem::path &)> set)
		: LiveValue<filesystem::path>(caption, get) {
			this->setFunction = set;
			//manually enable the edit button with our own actions
			// this might break if the code for LiveValue<T>::setEditable changes [2015-03-09]
			this->editButton->enable();
			this->editButton->onMouseReleased.addListener([this](MouseArguments &) {
				auto currentValue = this->liveValue();
				ofFileDialogResult result;
				
				bool isFolder = false;
				{
					const auto title = ofToLower(this->getCaption());
					isFolder = title.find("folder") != string::npos
						|| title.find("directory") != string::npos;
				}

				if (isFolder) {
					result = ofSystemLoadDialog(this->caption + " [" + currentValue.string() + "]", true, currentValue.string());
				}
				else {
					bool isSave = false;
					{
						const auto title = ofToLower(this->getCaption());
						isSave = title.find("save") != string::npos;
					}

					if (isSave) {
						result = ofSystemSaveDialog(currentValue.string(), this->caption + " [" + currentValue.string() + "]");
					}
					else {
						result = ofSystemLoadDialog(this->caption + " [" + currentValue.string() + "]", false, currentValue.string());
					}
				}
				
				
				if (result.bSuccess) {
					auto path = filesystem::path(result.filePath);
					this->setFunction(path);
					this->onValueChange.notifyListeners(path);
				}
			}, this);
		}

		//----------
		SelectFile::SelectFile(ofParameter<filesystem::path> & filename)
			: SelectFile(filename.getName()
				, [&filename]() {
					return filename;
				}
				, [&filename](filesystem::path & newFilename) { 
					filename = newFilename;
				}) {
		}
	}
}