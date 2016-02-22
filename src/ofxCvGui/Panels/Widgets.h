#pragma once

#include "Scroll.h"

#include "../Widgets/Button.h"
#include "../Widgets/EditableValue.h"
#include "../Widgets/Indicator.h"
#include "../Widgets/Slider.h"
#include "../Widgets/Title.h"
#include "../Widgets/Toggle.h"

#define OFXCVGUI_PANELS_WIDGETS_MAKE_HEADER(WidgetType, ...) shared_ptr<ofxCvGui::Widgets::T> add(__VA_ARGS__)
#define OFXCVGUI_PANELS_WIDGETS_MAKE_BODY(T, ...) auto widget = ofxCvGui::Widgets::T::make(__VA_ARGS__); \
	this->add(widget); \
	return widget;
#define OFXCVGUI_PANELS_WIDGETS_MAKE_SIMPLE(T, ARGTYPE) OFXCVGUI_PANELS_WIDGETS_MAKE_HEADER(T, ARGTYPE parameter) { OFXCVGUI_PANELS_WIDGETS_MAKE_BODY(T, parameter) }

namespace ofxCvGui {
	namespace Panels {
		class Widgets : public Scroll {
		public:
			using Scroll::add;
			
			// Add Fps Widget
			shared_ptr<ofxCvGui::Widgets::LiveValueHistory> addFps() {
				auto widget = ofxCvGui::Widgets::makeFps();
				this->add(widget);
				return widget;
			}

			// Add Widgets::Button
			shared_ptr<ofxCvGui::Widgets::Button> addButton(const string & caption, const function<void()> & action) {
				auto widget = ofxCvGui::Widgets::Button::make(caption, action);
				this->add(widget);
				return widget;
			}

			// Add Widgets::EditableValue
			template<typename ValueType>
			shared_ptr<ofxCvGui::Widgets::EditableValue<ValueType>> addEditableValue(
				const string & caption,
				const function<ValueType(void)> & get,
				const function<void(string)> & set) {
				auto widget = ofxCvGui::Widgets::EditableValue<ValueType>::make(caption, get, set);
				this->add(widget);
				return widget;
			}

			// Add Widgets::EditableValue
			template<typename ValueType>
			shared_ptr<ofxCvGui::Widgets::EditableValue<ValueType>> addEditableValue(
				ofParameter<ValueType> & parameter) {
				auto widget = ofxCvGui::Widgets::EditableValue<ValueType>::make(parameter);
				this->add(widget);
				return widget;
			}

			// Add Widgets::Indicator
			shared_ptr<ofxCvGui::Widgets::Indicator> addIndicator(const string & caption, const function<bool()> & get) {
				auto widget = ofxCvGui::Widgets::Indicator::make(caption, [get]() {
					if (get()) {
						return ofxCvGui::Widgets::Indicator::Status::Good;
					}
					else {
						return ofxCvGui::Widgets::Indicator::Status::Clear;
					}
				});
				this->add(widget);
				return widget;
			}

			// Add Widgets::LiveValue
			template<typename ValueType>
			shared_ptr<ofxCvGui::Widgets::LiveValue<ValueType>> addLiveValue(const string & caption, const function<ValueType(void)> & get) {
				auto widget = ofxCvGui::Widgets::LiveValue<ValueType>::make(caption, get);
				this->add(widget);
				return widget;
			}

			// Add Widgets::LiveValueHistory
			shared_ptr<ofxCvGui::Widgets::LiveValueHistory> addLiveValueHistory(const string & caption, const function<float(void)> & get) {
				auto widget = ofxCvGui::Widgets::LiveValueHistory::make(caption, get);
				this->add(widget);
				return widget;
			}

			// Add Widgets::Slider
			shared_ptr<ofxCvGui::Widgets::Slider> addSlider(ofParameter<float> & parameter) {
				auto widget = ofxCvGui::Widgets::Slider::make(parameter);
				this->add(widget);
				return widget;
			}

			// Add Widgets::Title
			shared_ptr<ofxCvGui::Widgets::Title> addTitle(const string & caption, ofxCvGui::Widgets::Title::Level level = ofxCvGui::Widgets::Title::Level::H1) {
				auto widget = ofxCvGui::Widgets::Title::make(caption, level);
				this->add(widget);
				return widget;
			}

			// Add Widgets::Toggle
			shared_ptr<ofxCvGui::Widgets::Toggle> addToggle(ofParameter<bool> & parameter) {
				auto widget = ofxCvGui::Widgets::Toggle::make(parameter);
				this->add(widget);
				return widget;
			}

			// Add Widgets::Toggle
			shared_ptr<ofxCvGui::Widgets::Toggle> addToggle(const string & caption, const function<bool()> & get, const function<void(bool)> & set) {
				auto widget = ofxCvGui::Widgets::Toggle::make(caption, get, set);
				this->add(widget);
				return widget;
			}
		};

		shared_ptr<Panels::Widgets> makeWidgets(string caption = "");
	}
}