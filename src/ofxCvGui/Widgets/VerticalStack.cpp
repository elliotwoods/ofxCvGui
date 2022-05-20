#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		VerticalStack::VerticalStack()
		{
			// Default height (just in-case)
			this->setHeight(80.0f);

			// when resizing, arrange children
			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments& args) {
				if (args.localBounds.width == 0.0f) {
					// Not ready yet
					return;
				}
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
			};
		}
	}
}