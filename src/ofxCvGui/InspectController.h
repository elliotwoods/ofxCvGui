#pragma once

#include "ElementGroup.h"

namespace ofxCvGui {
	/// inherit this class if your object is inspectable
	class IInspectable {
	public:
		/// override this function to populate an inspector when selected
		virtual void populateInspector(ElementGroupPtr) = 0;
	};

	class InspectController {
	public:
		InspectController();

		///singleton
		static InspectController & X();

		void update();

		/// The first person to call this per frame wins the inspector.
		/// This is to reflect the inverse call order of mouse action trees (top first)
		void inspect(shared_ptr<IInspectable>);
		void clear();
		shared_ptr<IInspectable> getTarget() const;

		void addToInspector(ElementPtr);

		ofxLiquidEvent<shared_ptr<IInspectable>> onTargetChange;
		ofxLiquidEvent<ElementPtr> onAddWidget;

		///this is actually triggered from the individual inspectors
		ofxLiquidEvent<ElementGroupPtr> onClear;
	protected:
		static InspectController * singleton;
		weak_ptr<IInspectable> currentTarget;
		bool hasTarget;

		shared_ptr<IInspectable> inspectThisFrame;
		bool clearThisFrame;
	};

	void inspect(shared_ptr<IInspectable>);

	bool isBeingInspected(shared_ptr<IInspectable>);
	bool isBeingInspected(IInspectable &);

	void addToInspector(ofxCvGui::ElementPtr);
}