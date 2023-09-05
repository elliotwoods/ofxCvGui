#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		VerticalStack::VerticalStack(const Layout& layout)
			: layout(layout)
		{
			// Default height (just in-case)
			this->setHeight(80.0f);

			// when resizing, arrange children
			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments& args) {
				if (args.localBounds.width == 0.0f) {
					// Not ready yet
					return;
				}

				switch (this->layout) {
				case Layout::DistributeEvenly:
				{
					auto heightPerElement = args.localBounds.height / this->elements.size();

					float y = 0;
					// arrange children in a col grid
					for (const auto& element : this->elements) {
						element->setBounds({
							0, y
							, args.localBounds.width, heightPerElement - 10
							});
						y += heightPerElement;
					}
					break;
				}
				case Layout::UseElementHeight:
				{
					float y = 0;
					// arrange children in a col grid
					for (const auto& element : this->elements) {
						element->setBounds({
							0, y
							, args.localBounds.width, element->getHeight()
							});
						y += element->getHeight();
					}
					break;
				}
				}
			};
		}
	}
}