#include "Inspector.h"
#include "../Widgets/IInspectable.h"
#include "../Widgets/Title.h"

namespace ofxCvGui {
	using namespace Widgets;

	namespace Panels {
		//---------
		Inspector::Inspector() {
			Inspector::onNewSelection += [this] (IInspectable & object) {
				this->clear();
				object.populate(this->elements);
				this->arrange();
			};

			this->clear();
		}

		//---------
		void Inspector::clear() {
			this->elements->clear();
			this->elements->add(shared_ptr<Title>(new Title("Inspector", Title::Level::H1)));
			this->onClear(this->elements);
		}

		//---------
		void Inspector::select(IInspectable & object) {
			Inspector::onNewSelection(object);
		}

		//---------
		ofxLiquidEvent<ElementGroupPtr> Inspector::onClear = ofxLiquidEvent<ElementGroupPtr>();

		//---------
		ofxLiquidEvent<IInspectable> Inspector::onNewSelection = ofxLiquidEvent<IInspectable>();
	}
}