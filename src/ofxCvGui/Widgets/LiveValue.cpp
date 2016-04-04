#include "LiveValue.h"

#include "ofAppRunner.h"

#ifdef TARGET_WIN32
	#include "psapi.h"
#endif
#ifdef  TARGET_OSX
	#include <mach/vm_statistics.h>
	#include <mach/mach_types.h>
	#include <mach/mach_init.h>
	#include <mach/mach_host.h>
#endif


namespace ofxCvGui {
	namespace Widgets {
		bool isFinite(float number) {
			if (number != number) {
				//NaN case
				return false;
			}
			if (number + 1 == number){
				//infinite case
				return false;
			}
			return true;
		}

		//----------
		LiveValueHistory::LiveValueHistory(string caption, function<float()> liveValue, bool keepZeroAsMinimum) :
		LiveValue(caption, liveValue) {
			this->setScissor(true);

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

				//damp towards new bounds
				if (!this->keepZeroAsMinimum) {
					if (isFinite(this->minimum)) {
						this->minimum = this->minimum * 0.8 + newMin * 0.2;
					}
					else {
						this->minimum = newMin;
					}
				}
				if (isFinite(this->maximum)) {
					this->maximum = this->maximum * 0.8 + newMax * 0.2;
				}
				else {
					this->maximum = newMax;
				}

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
					float y = ofMap(point, this->minimum, this->maximum, 40.0f, 0.0f, false);

					//check valid number
					if (y != y || y <= std::numeric_limits<float>::min() || y >= std::numeric_limits<float>::max()) {
						continue;
					}
					
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
						ofxAssets::font(ofxCvGui::getDefaultTypeface(), 14).drawString(previewValueString, 0, 35);
						ofDrawCircle(this->hoverX, ofMap(value, this->minimum, this->maximum, 40.0f, 0.0f), 3.0f);
						ofPopStyle();
					}
				}
			}, -1, this);

			this->onMouse += [this] (MouseArguments & args) {
				if (args.local.y < 40) {
					if (args.takeMousePress(this)) {
						switch (args.button) {
						case 0:
							this->pause ^= true;
							if (this->pause) {
								this->graphLine.clear();
								this->graphFill.setFillColor(80);
							}
							break;
						case 2:
							this->history.clear();
							this->minimum = 0.0f;
							this->maximum = 1.0f;
							break;
						}
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
		
		//----------
		shared_ptr<LiveValueHistory> makeFps() {
			return shared_ptr<LiveValueHistory>(new LiveValueHistory("Framerate", [] () {
				return ofGetFrameRate();
			}));
		}

		//----------
		shared_ptr<LiveValueHistory> makeMemoryUsage() {
			return shared_ptr<LiveValueHistory>(new LiveValueHistory("Memory usage [MB]", []() {
#ifdef TARGET_WIN32
				PROCESS_MEMORY_COUNTERS pmc;
				GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
				return (float)pmc.WorkingSetSize / 1e6;
#endif
#ifdef TARGET_OSX
				struct task_basic_info t_info;
				mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

				if (KERN_SUCCESS != task_info(mach_task_self(),
					TASK_BASIC_INFO, (task_info_t)&t_info,
					&t_info_count))
				{
					return 0.0f;
				}
				else {
					return (float) t_info.resident_size / 1000000.0f;
				}
#endif
			}));
		}
	}
}