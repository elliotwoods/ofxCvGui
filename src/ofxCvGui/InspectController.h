#pragma once

#include "ElementGroup.h"

namespace ofxCvGui {
	struct InspectArguments {
		ElementGroupPtr inspector;
	};

	/// inherit this class if your object is inspectable
	class IInspectable {
	public:
		/// Listen to this event if you want to populate the insspector when selected 
		ofxLiquidEvent<InspectArguments> onInspect;
		bool isBeingInspected() const;
	};

	class InspectController {
	public:
		///singleton
		static InspectController & X();
		static void setSingleton(shared_ptr<InspectController>);
		static shared_ptr<InspectController> getSingleton();
	protected:
		static shared_ptr<InspectController> singleton;
	public:
		InspectController();

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
		ofxLiquidEvent<InspectArguments> onClear;
	protected:
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