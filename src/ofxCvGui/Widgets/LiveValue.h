#pragma once
#include "../Element.h"
#include "ofParameter.h"
#include "../../../addons/ofxAssets/src/ofxAssets.h"

namespace ofxCvGui {
	namespace Widgets {
		template<typename T>
		class LiveValue : public Element {
		public:
			OFXCVGUI_MAKE_ELEMENT_HEADER(LiveValue<T>, string caption, const function<T()> & liveValueFunction) {
				OFXCVGUI_MAKE_ELEMENT_BODY(LiveValue<T>, caption, liveValueFunction);
			}

			LiveValue(string caption, function<T()> liveValue) {
				this->setCaption(caption);
				this->liveValue = liveValue;
				this->setBounds(ofRectangle(5, 0, 100, 50));

				this->onUpdate += [this] (UpdateArguments &) {
					stringstream ss;
					ss << this->liveValue();
					this->result = ss.str();
				};

				this->onDraw += [this] (DrawArguments & args) {
					auto & captionFont = ofxAssets::AssetRegister.getFont(ofxCvGui::defaultTypeface, 12);
					captionFont.drawString(this->caption + " : ", 0, 15);

					auto & valueFont = ofxAssets::AssetRegister.getFont(ofxCvGui::defaultTypeface, 14);
					auto valueBounds = valueFont.getStringBoundingBox(result, 0, 0);
					valueFont.drawString(result, (int) (this->getWidth() - valueBounds.width - 5), 35);

					ofPushStyle();
					ofSetLineWidth(1.0f);
					ofLine(this->getWidth(), 0, this->getWidth(), 40);
					ofPopStyle();	
				};
			};

			virtual ~LiveValue() { }
		protected:
			void init();
			void update(UpdateArguments &);
			void draw(DrawArguments &);

			function<T()> liveValue;
			string result;
		};

		class LiveValueHistory : public LiveValue<float> {
		public:
			OFXCVGUI_MAKE_ELEMENT_HEADER(LiveValueHistory, string caption, const function<float()> & liveValueFunction, bool keepZeroAsMinimum) {
				OFXCVGUI_MAKE_ELEMENT_BODY(LiveValueHistory, caption, liveValueFunction, keepZeroAsMinimum);
			}

			LiveValueHistory(string caption, function<float()> liveValue, bool keepZeroAsMinimum = false);
		protected:
			deque<float> history;
			bool keepZeroAsMinimum;
			float minimum;
			float maximum;
			ofPath graphFill;
			ofPolyline graphLine;

			bool showPreviewValue;
			float hoverX;

			bool pause;
		};
		
		shared_ptr<LiveValueHistory> makeFps();
	}
}