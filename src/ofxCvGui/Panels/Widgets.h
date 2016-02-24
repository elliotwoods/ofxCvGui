#pragma once

#include "Scroll.h"

#include "../Widgets/Button.h"
#include "../Widgets/EditableValue.h"
#include "../Widgets/Indicator.h"
#include "../Widgets/MultipleChoice.h"
#include "../Widgets/Slider.h"
#include "../Widgets/Spacer.h"
#include "../Widgets/Title.h"
#include "../Widgets/Toggle.h"

namespace ofxCvGui {
	namespace Panels {
		class Widgets : public Scroll {
		public:
			// Widgets::FPS
			shared_ptr<ofxCvGui::Widgets::LiveValueHistory> addFps();

			// Widgets::Button
			shared_ptr<ofxCvGui::Widgets::Button> addButton(const string & caption, const function<void()> & action);

			// Widgets::EditableValue<ValueType>
			template<typename ValueType>
			shared_ptr<ofxCvGui::Widgets::EditableValue<ValueType>> addEditableValue(
				const string & caption,
				const function<ValueType(void)> & get,
				const function<void(string)> & set) {
				return this->add(new ofxCvGui::Widgets::EditableValue<ValueType>(caption, get, set));
			}

			// Widgets::EditableValue<ValueType>
			template<typename ValueType>
			shared_ptr<ofxCvGui::Widgets::EditableValue<ValueType>> addEditableValue(
				ofParameter<ValueType> & parameter) {
				return this->add(new ofxCvGui::Widgets::EditableValue<ValueType>(parameter));
			}

			// Widgets::Indicator
			shared_ptr<ofxCvGui::Widgets::Indicator> addIndicator(const string & caption, const function<bool()> & get);

			// Widgets::LiveValue<ValueType>
			template<typename ValueType>
			shared_ptr<ofxCvGui::Widgets::LiveValue<ValueType>> addLiveValue(const string & caption, const function<ValueType(void)> & get) {
				return this->add(new ofxCvGui::Widgets::LiveValue<ValueType>(caption, get));
			}

			// Widgets::LiveValueHistory
			shared_ptr<ofxCvGui::Widgets::LiveValueHistory> addLiveValueHistory(const string & caption, const function<float(void)> & get);
			
			// Widgets::MultipleChoice
			shared_ptr<ofxCvGui::Widgets::MultipleChoice> addMultipleChoice(const string & caption);

			// Widgets::MultipleChoice
			shared_ptr<ofxCvGui::Widgets::MultipleChoice> addMultipleChoice(const string & caption, const initializer_list<string> & options);

			// Widgets::Slider
			shared_ptr<ofxCvGui::Widgets::Slider> addSlider(ofParameter<float> & parameter);
			
			// Widgets::Spacer
			shared_ptr<ofxCvGui::Widgets::Spacer> addSpacer();

			// Widgets::Title
			shared_ptr<ofxCvGui::Widgets::Title> addTitle(const string & caption, ofxCvGui::Widgets::Title::Level level = ofxCvGui::Widgets::Title::Level::H1);

			// Widgets::Toggle
			shared_ptr<ofxCvGui::Widgets::Toggle> addToggle(ofParameter<bool> & parameter);

			// Add Widgets::Toggle
			shared_ptr<ofxCvGui::Widgets::Toggle> addToggle(const string & caption, const function<bool()> & get, const function<void(bool)> & set);
		};

		shared_ptr<Panels::Widgets> makeWidgets(string caption = "");
	}
}