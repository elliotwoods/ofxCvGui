#include "pch_ofxCvGui.h"

using namespace std;
using namespace ofxAssets;

namespace ofxCvGui {
	namespace Panels {
		//-----------
		Base::Base() {
			this->setScissorEnabled(true);
			
			this->toolBar = makeElementGroup();
			this->addChild(this->toolBar);
			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments& args) {
				this->toolBar->setBounds(args.localBounds);
			};
			this->toolBar->setScissorEnabled(true);
			this->toolBar->setCaption("Toolbar"); // For debug purposes
			
			auto titleElement = makeElement();
			titleElement->setScissorEnabled(false);
			titleElement->onDraw += [this](DrawArguments & args) {
				if (!this->caption.empty()) {
					Utils::drawText(this->caption, 0, 0, true, 30);
				}
			};
			this->toolBar->add(titleElement);
			
			this->toolBar->onGroupChange += [this, titleElement]() {
				float x = 20;
				float y = 20;
				for(auto element : this->toolBar->getElements()) {
					if(element == titleElement) {
						continue;
					}
					element->setBounds(ofRectangle(x, y, 30, 30));
					x += 40;
				}
				titleElement->setBounds(ofRectangle(x, y, 30, 30)); //title has no scissor so width is fine
			};

			this->onUpdate += [this](ofxCvGui::UpdateArguments&) {
				this->toolBar->setEnabled(this->chromeEnabled);
			};
		}

		//----------
		ElementPtr Base::addToolBarElement(const string &imageAssetName, function<void ()> && action) {
			auto element = makeElement();
			element->onDraw += [imageAssetName](DrawArguments & args) {
				ofxAssets::image(imageAssetName).draw(args.localBounds);
			};
			element->onMouseReleased += [action](MouseArguments &) {
				action();
			};
			this->toolBar->add(element);
			return element;
		}

		//----------
		void Base::setChromeEnabled(bool chromeEnabled) {
			this->chromeEnabled = chromeEnabled;
		}

		//----------
		bool Base::getChromeEnabled() const {
			return this->chromeEnabled;
		}
		
		//----------
		PanelPtr makeBlank(string caption) {
			auto newPanel = make_shared<Base>();
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}