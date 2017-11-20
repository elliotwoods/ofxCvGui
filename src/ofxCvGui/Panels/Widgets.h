#pragma once

#include "Scroll.h"

#include "../Widgets/Button.h"
#include "../Widgets/EditableValue.h"
#include "../Widgets/Indicator.h"
#include "../Widgets/MultipleChoice.h"
#include "../Widgets/Slider.h"
#include "../Widgets/Spacer.h"
#include "../Widgets/Title.h"
#include "../Widgets/SelectFile.h"
#include "../Widgets/Toggle.h"

#include "ofParameterGroup.h"
#include "ofxSingleton.h"

namespace ofxCvGui {
	namespace Panels {
		class Widgets : public Scroll {
		public:
			// Element
			shared_ptr<ofxCvGui::Element> addBlank();

			// Widgets::LiveValueHistory [FPS]
			shared_ptr<ofxCvGui::Widgets::LiveValueHistory> addFps();

			// Widgets::LiveValueHistory [Memory]
			shared_ptr<ofxCvGui::Widgets::LiveValueHistory> addMemoryUsage();

			// Widgets::Button
			shared_ptr<ofxCvGui::Widgets::Button> addButton(const string & caption, const function<void()> & action, char hotKey = 0);

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
			shared_ptr<ofxCvGui::Widgets::Indicator> addIndicatorBool(const string & caption, const function<bool()> & get);
			shared_ptr<ofxCvGui::Widgets::Indicator> addIndicator(const string & caption, const function<ofxCvGui::Widgets::Indicator::Status()> & get);

			// Widgets::LiveValue<ValueType>
			template<typename ValueType>
			shared_ptr<ofxCvGui::Widgets::LiveValue<ValueType>> addLiveValue(const string & caption, const function<ValueType(void)> & get) {
				return this->add(new ofxCvGui::Widgets::LiveValue<ValueType>(caption, get));
			}

			// Widgets::LiveValue<ValueType>
			template<typename ValueType>
			shared_ptr<ofxCvGui::Widgets::LiveValue<ValueType>> addLiveValue(ofParameter<ValueType> & parameter) {
				return this->add(new ofxCvGui::Widgets::LiveValue<ValueType>(parameter.getName(), [& parameter]() {
					return parameter.get();
				}));
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
			shared_ptr<ofxCvGui::Widgets::Spacer> addSpacer(bool preventDoubles = true);

			// Widgets::Title
			shared_ptr<ofxCvGui::Widgets::Title> addTitle(const string & caption, ofxCvGui::Widgets::Title::Level level = ofxCvGui::Widgets::Title::Level::H1);

			// Widgets::Toggle
			shared_ptr<ofxCvGui::Widgets::Toggle> addToggle(ofParameter<bool> & parameter);

			// Add Widgets::Toggle
			shared_ptr<ofxCvGui::Widgets::Toggle> addToggle(const string & caption, const function<bool()> & get, const function<void(bool)> & set);

			// Add Widgets::SelectFile
			shared_ptr<ofxCvGui::Widgets::SelectFile> addFilePath(ofParameter<filesystem::path> & parameter);

			// Add ofParameterGroup
			void addParameterGroup(ofParameterGroup &, int titleLevel = 0);
		};

		class WidgetsBuilder : public ofxSingleton::UnmanagedSingleton<WidgetsBuilder> {
		public:
			typedef function<ElementPtr(shared_ptr<ofAbstractParameter>)> BuildFunction;
			template<typename Type>
			void registerBuilder(function<ElementPtr(ofParameter<Type> & parameter)> buildFunction) {
				auto typeIdHash = typeid(Type).hash_code();
				auto entry = this->wrappedBuildFunctions.find(typeIdHash);
				if (entry == this->wrappedBuildFunctions.end()) {
					this->wrappedBuildFunctions[typeIdHash] = [buildFunction](shared_ptr<ofAbstractParameter> parameter) {
						auto tryCast = dynamic_pointer_cast<ofParameter<Type>>(parameter);
						if (tryCast) {
							return buildFunction(*tryCast);
						}
						else {
							return ElementPtr();
						}
					};
				}
			}

			ElementPtr tryBuild(shared_ptr<ofAbstractParameter>);
		protected:
			map<size_t, BuildFunction> wrappedBuildFunctions;
		};

		shared_ptr<Panels::Widgets> makeWidgets(string caption = "");
	}
}