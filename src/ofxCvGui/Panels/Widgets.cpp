#include "Widgets.h"

using namespace ofxCvGui::Widgets;

namespace ofxCvGui {
	namespace Panels {
		//----------
		shared_ptr<ofxCvGui::Widgets::LiveValueHistory> Widgets::addFps() {
			auto widget = ofxCvGui::Widgets::makeFps();
			this->add(widget);
			return widget;
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Button> Widgets::addButton(const string & caption, const function<void()> & action) {
			return this->add(new ofxCvGui::Widgets::Button(caption, action));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Indicator> Widgets::addIndicator(const string & caption, const function<bool()> & get) {
			return this->add(new ofxCvGui::Widgets::Indicator(caption, [get]() {
				if (get()) {
					return ofxCvGui::Widgets::Indicator::Status::Good;
				}
				else {
					return ofxCvGui::Widgets::Indicator::Status::Clear;
				}
			}));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::LiveValueHistory> Widgets::addLiveValueHistory(const string & caption, const function<float(void)> & get) {
			return this->add(new ofxCvGui::Widgets::LiveValueHistory(caption, get));
		}


		//----------
		shared_ptr<ofxCvGui::Widgets::MultipleChoice> Widgets::addMultipleChoice(const string & caption) {
			return this->add(new ofxCvGui::Widgets::MultipleChoice(caption));
		}


		//----------
		shared_ptr<ofxCvGui::Widgets::MultipleChoice> Widgets::addMultipleChoice(const string & caption, const initializer_list<string> & options) {
			return this->add(new ofxCvGui::Widgets::MultipleChoice(caption, options));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Slider> Widgets::addSlider(ofParameter<float> & parameter) {
			return this->add(new ofxCvGui::Widgets::Slider(parameter));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Spacer> Widgets::addSpacer() {
			return this->add(new ofxCvGui::Widgets::Spacer());
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Title> Widgets::addTitle(const string & caption, ofxCvGui::Widgets::Title::Level level) {
			return this->add(new ofxCvGui::Widgets::Title(caption, level));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Toggle> Widgets::addToggle(ofParameter<bool> & parameter) {
			return this->add(new ofxCvGui::Widgets::Toggle(parameter));
		}

		//----------
		shared_ptr<ofxCvGui::Widgets::Toggle> Widgets::addToggle(const string & caption, const function<bool()> & get, const function<void(bool)> & set) {
			return this->add(new ofxCvGui::Widgets::Toggle(caption, get, set));
		}

		//----------
		shared_ptr<Panels::Widgets> makeWidgets(string caption) {
			auto newPanel = shared_ptr<Panels::Widgets>(new Panels::Widgets());
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}