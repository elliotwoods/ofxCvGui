#pragma once
#include "../Element.h"
#include "ofParameter.h"
#include "ofSystemUtils.h"
#include "../../../addons/ofxAssets/src/ofxAssets.h"

namespace ofxCvGui {
	namespace Widgets {
		template<typename T>
		class LiveValue : public Element {
		public:
			LiveValue(string caption, function<T()> liveValue) {
				this->init(caption, liveValue);
			};

			LiveValue(string caption, const T & liveValue) {
				this->init(caption, [&liveValue]() {
					return liveValue;
				});
			};

			void init(string caption, function<T()> liveValue) {
				this->setCaption(caption);
				this->liveValue = liveValue;
				this->setBounds(ofRectangle(0, 0, 100, 40));

				this->onUpdate += [this](UpdateArguments &) {
					stringstream ss;
					ss << this->liveValue();
					this->cachedValue = ss.str();
				};
				this->onDraw += [this](DrawArguments & args) {
					auto & captionFont = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 12);
					captionFont.drawString(this->caption + " : ", 0, 15);

					auto & valueFont = ofxAssets::font(ofxCvGui::getDefaultTypeface(), 14);
					auto valueBounds = valueFont.getStringBoundingBox(cachedValue, 0, 0);
					valueFont.drawString(cachedValue, (int)(this->getWidth() - valueBounds.width - 5), 35);
				};
				this->onBoundsChange += [this](BoundsChangeArguments & args) {
					this->editButton->setBounds(ofRectangle(args.localBounds.width - 20, 5, 15, 15));
				};

				this->editButton = make_shared<Element>();
				this->editButton->onDraw += [this](DrawArguments & args) {
					ofxAssets::image("ofxCvGui::edit").draw(args.localBounds);
				};
				this->editButton->addListenersToParent(this);
				this->setEditable(false);
			}


			virtual ~LiveValue() { }

			void setEditable(bool editable) {
				this->editButton->setEnabled(editable);
				if (editable) {
					this->editButton->onMouseReleased.removeListeners(this);
					this->editButton->onMouseReleased.addListener([this](MouseArguments & args) {
						this->hitEditBox();
					}, this);
				}
			}
			void hitEditBox() {
				auto result = ofSystemTextBoxDialog("Set [" + this->getCaption() + "] (" + this->cachedValue + ")");
				if (result != "") {
					this->onEditValue(result);
				}
			}

			ofxLiquidEvent<string> onEditValue;

		protected:
			function<T()> liveValue;
			string cachedValue;
			
			shared_ptr<Element> editButton;
		};
		
		template<> void LiveValue<string>::hitEditBox() {
			auto result = ofSystemTextBoxDialog("Set [" + this->getCaption() + "]");
			this->onEditValue(result);
		}

		class LiveValueHistory : public LiveValue<float> {
		public:
			LiveValueHistory(string caption, function<float()> liveValue, bool keepZeroAsMinimum = true);
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