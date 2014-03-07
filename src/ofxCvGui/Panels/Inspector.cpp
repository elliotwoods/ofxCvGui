#include "Inspector.h"
#include "../Widgets/IInspectable.h"
#include "../Widgets/Title.h"

namespace ofxCvGui {
	using namespace Widgets;

	namespace Panels {
		//---------
		Inspector::Inspector() {
			Inspector::makeNewSelection += [this] (IInspectable & object) {
				this->clear();
				object.populate(this->elements);
				this->arrange();
			};

			Inspector::makeNoSelection += [this] (IInspectable & object) {
				this->clear();
			};

			this->initialised = false;
			this->onUpdate += [this] (UpdateArguments &) {
				if (!this->initialised) {
					this->clear();
					this->initialised = true;
				}
			};

			this->clear();
		}

		//---------
		void Inspector::clear() {
			Inspector::selection.clear();
			this->elements->clear();
			this->elements->add(shared_ptr<Title>(new Title("Inspector", Title::Level::H1)));
			this->onClear(this->elements);
			this->arrange();
		}

		//---------
		void Inspector::select(IInspectable & object) {
			if (Inspector::isSelected(object)) {
				Inspector::makeNoSelection(object);
				Inspector::selection.clear();
			} else {
				Inspector::makeNewSelection(object);
				Inspector::selection.insert(&object);
			}
		}

		//---------
		bool Inspector::isSelected(Widgets::IInspectable & object) {
			return Inspector::selection.count(&object) > 0;
		}

		//---------
		ofxLiquidEvent<ElementGroupPtr> Inspector::onClear = ofxLiquidEvent<ElementGroupPtr>();

		//---------
		ofxLiquidEvent<IInspectable> Inspector::makeNewSelection = ofxLiquidEvent<IInspectable>();
		ofxLiquidEvent<IInspectable> Inspector::makeNoSelection = ofxLiquidEvent<IInspectable>();

		//---------
		set<Widgets::IInspectable *> Inspector::selection = set<Widgets::IInspectable *>();
	}
}
