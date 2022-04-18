#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		HorizontalStack::HorizontalStack()
		{
			// Default height (just in-case)
			this->setHeight(40.0f);

			// All widgets need to have a height, so we take the max of the child's heights
			this->onGroupChange += [this]() {
				this->needsNewHeight = true;
			};
			this->onUpdate += [this](ofxCvGui::UpdateArguments&) {
				if (this->needsNewHeight) {
					float maxHeight = 40.0f;
					for (const auto element : this->elements) {
						maxHeight = max(maxHeight, element->getHeight());
					}
					this->setHeight(maxHeight);

					this->needsNewHeight = false;
				}
			};

			// when resizing, arrange children
			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments& args) {
				if (args.localBounds.height == 0.0f) {
					// Not ready yet
					return;
				}
				auto widthPerElement = args.localBounds.width / this->elements.size();

				float x = 0;
				// arrange children in a col grid
				for (const auto & element : this->elements) {
					element->setBounds({
						x, 0
						, widthPerElement - 10, args.localBounds.height
						});
					x += widthPerElement;
				}
			};
		}
	}
}