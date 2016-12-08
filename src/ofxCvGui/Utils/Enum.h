#pragma once

#include "ofxCvGui/Panels/Widgets.h"
#include "ofxCvGui/Widgets/MultipleChoice.h"

#include <string.h>
#include <vector>

using namespace std;

namespace ofxCvGui {
	class BaseEnum {
	public:
		virtual const vector<string> & getOptionStrings() const = 0;
	};
}

#define OFXCVGUI_UNWRAP(...) __VA_ARGS__

#define MAKE_ENUM(NAME, OPTIONS, STRINGS) \
class NAME : public ofxCvGui::BaseEnum { \
public: \
	enum Options : uint32_t { OFXCVGUI_UNWRAP OPTIONS } ;\
	const vector<string> & getOptionStrings() const override { \
		return this->optionStrings; \
	} \
	NAME() { this->innerValue = (Options) 0; this->registerBuilder(); } \
	NAME(Options value) { this->innerValue = value; this->registerBuilder(); } \
	Options get() const { \
		return this->innerValue; \
	} \
	void set(Options value) { \
		this->innerValue = value; \
	} \
	void operator=(Options value) { \
		this->set(value); \
	} \
	operator const Options &() const { \
		return this->innerValue; \
	} \
	operator Options &() { \
		return this->innerValue; \
	} \
protected: \
	void registerBuilder() { \
		auto & optionStrings = this->optionStrings; \
		ofxCvGui::Panels::WidgetsBuilder::X().registerBuilder<NAME>([optionStrings](ofParameter<NAME> & parameter){ \
			auto widget = make_shared<ofxCvGui::Widgets::MultipleChoice>(parameter.getName());\
			for(auto & option : optionStrings) { \
				widget->addOption(option); \
			} \
			widget->setSelection((int) parameter.get().get()); \
			auto weakWidget = weak_ptr<ofxCvGui::Widgets::MultipleChoice>(widget); \
			widget->onUpdate += [&parameter, weakWidget](ofxCvGui::UpdateArguments &) { \
				auto widget = weakWidget.lock(); \
				if(widget) { \
					if(widget->getSelectionIndex() != (int) parameter.get().get()) { \
						widget->setSelection((int) parameter.get().get()); \
					} \
				} \
			}; \
			widget->onValueChange += [&parameter](int value) { \
				if((int) parameter.get().get() != value) { \
					parameter.set((Options) value); \
				} \
			};\
			return widget; \
		}); \
	} \
	Options innerValue; \
	vector<string> optionStrings { OFXCVGUI_UNWRAP STRINGS }; \
};
