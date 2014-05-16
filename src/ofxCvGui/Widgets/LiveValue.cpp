#include "LiveValue.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		LiveValueHistory::LiveValueHistory(string caption, function<float()> liveValue, bool keepZeroAsMinimum) :
		LiveValue(caption, liveValue) {
			this->minimum = 0.0f;
			this->maximum = 1.0f;
			this->keepZeroAsMinimum = keepZeroAsMinimum;
			this->showPreviewValue = false;
			this->pause = false;
			
			this->onUpdate += [this] (UpdateArguments &) {
				if (this->pause) {
					return;
				}

				this->history.push_back(this->liveValue());
				auto size = this->history.size();
				while (size > this->getWidth()) {
					this->history.pop_front();
					size--;
				}

				this->graphFill.clear();
				this->graphLine.clear();
				if (size == 0) {
					return;
				}
				float newMin = std::numeric_limits<float>::max();
				float newMax = std::numeric_limits<float>::min();
				for(auto & point : history) {
					if (!this->keepZeroAsMinimum) {
						if (point < newMin) {
							newMin = point;
						}
					}
					if (point > newMax) {
						newMax = point;
					}
				}

				//curve towards new bounds
				if (!this->keepZeroAsMinimum) {
					this->minimum = this->minimum * 0.8 + newMin * 0.2;
				}
				this->maximum = this->maximum * 0.8 + newMax * 0.2;

				//check we have valid min and max
				if (this->minimum != this->minimum || this->maximum != this->maximum) {
					this->minimum = 0.0f;
					this->maximum = 1.0f;
				}

				//don't draw if we have no range
				if (abs(this->minimum - this->maximum) < FLT_EPSILON) {
					return;
				}

				this->graphFill.setFillColor(50);
				this->graphFill.lineTo(this->getWidth() - size, 40.0f);
				int x = 0;
				for(auto & point : this->history) {
					auto y = ofMap(point, this->minimum, this->maximum, 40.0f, 0.0f, false);
					this->graphFill.lineTo(this->getWidth() - size + x, y);
					this->graphLine.lineTo(this->getWidth() - size + x, y);
					x++;
				}
				this->graphFill.lineTo(this->getWidth(), 40.0f);
				this->graphFill.lineTo(this->getWidth() - size, 40.0f);
				this->graphFill.close();
			};
			this->onDraw.addListener([this] (DrawArguments & args) {
				ofPushStyle();
				
				ofFill();
				ofSetLineWidth(0);
				this->graphFill.draw();

				ofNoFill();
				ofSetLineWidth(1.0f);
				ofSetColor(80);
				this->graphLine.draw();

				ofPopStyle();

				if (this->showPreviewValue && (abs(this->minimum - this->maximum) > FLT_EPSILON)) {
					const auto historySize = this->history.size();
					unsigned int backX = this->getWidth() - this->hoverX;
					if (backX < historySize) {
						const auto value = this->history.at(historySize - backX - 1);
						auto previewValueString = ofToString(value);
						ofPushStyle();
						ofSetColor(150);
						ofxAssets::AssetRegister.getFont(ofxCvGui::defaultTypeface, 14).drawString(previewValueString, 0, 35);
						ofCircle(this->hoverX, ofMap(value, this->minimum, this->maximum, 40.0f, 0.0f), 3.0f);
						ofPopStyle();
					}
				}
			}, -1, this);
			this->onMouse += [this] (MouseArguments & args) {
				if (args.isLocalPressed() && args.local.y < 40) {
					if (args.button == 0) {
						this->pause ^= true;
						if (this->pause) {
							this->graphLine.clear();
							this->graphFill.setFillColor(80);
						}
						args.take();
					} else if (args.button == 2) {
						this->history.clear();
						this->minimum = 0.0f;
						this->maximum = 1.0f;
						args.take();
					}
				}
				if (args.action == MouseArguments::Action::Moved) {
					this->showPreviewValue = args.isLocal() && args.local.y < 40;
					if (this->showPreviewValue) {
						this->hoverX = args.local.x;
					}
				}
			};
		}
	}
}