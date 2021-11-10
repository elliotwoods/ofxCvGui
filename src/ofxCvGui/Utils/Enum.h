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
		virtual string toString() const = 0;
		virtual uint32_t toIndex() const = 0;
		virtual bool fromString(const string&) = 0;
		virtual bool fromIndex(const uint32_t&) = 0;
	};
}

#define OFXCVGUI_UNWRAP(...) __VA_ARGS__

#define MAKE_ENUM(NAME, OPTIONS, STRINGS) \
class NAME : public ofxCvGui::BaseEnum { \
public: \
	enum Options : uint32_t { OFXCVGUI_UNWRAP OPTIONS } ;\
	NAME() { this->innerValue = (Options) 0; this->registerBuilder(); } \
	NAME(Options value) { this->innerValue = value; this->registerBuilder(); } \
	void operator()(const NAME & other) { \
		this->innerValue = other.innerValue; \
	} \
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
	const vector<string> & getOptionStrings() const override { \
		return this->optionStrings; \
	} \
	string toString() const override { \
		return this->optionStrings.at(this->innerValue); \
	} \
	uint32_t toIndex() const override { \
		return (uint32_t) this->innerValue; \
	} \
	bool fromString(const string & valueString) { \
		for (uint32_t optionIndex = 0; optionIndex < this->optionStrings.size(); optionIndex++) { \
			const auto & optionString = this->optionStrings[optionIndex]; \
			if (optionString == valueString) { \
				this->set((Options)optionIndex); \
				return true; \
			} \
		} \
		return false; \
	} \
	bool fromIndex(const uint32_t& index) { \
		if(index < this->optionStrings.size()) { \
			this->set((Options)index); \
			return true; \
		} \
		return false; \
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



ostream& operator<<(ostream& stream, const ofxCvGui::BaseEnum& value);
istream& operator>>(istream& stream, ofxCvGui::BaseEnum& value);