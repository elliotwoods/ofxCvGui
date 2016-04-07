#pragma once

#include "ofxCvGui/Panels/Inspector.h"

#include "ElementGroup.h"
#include "ofxSingleton.h"

namespace ofxCvGui {
	struct InspectArguments {
		shared_ptr<Panels::Inspector> inspector;
	};

	/// inherit this class if your object is inspectable
	class IInspectable {
	public:
		/// Listen to this event if you want to populate the insspector when selected 
		ofxLiquidEvent<InspectArguments> onPopulateInspector;
		bool isBeingInspected() const;
	};

	/// The inspect controller determines what is being inspected and fires events
	///  when the target changes (e.g. these events are subscribed to by an Inspector).
	class InspectController : public ofxSingleton::Singleton<InspectController> {
	public:
		InspectController();

		void update();

		/// The first person to call this per frame wins the inspector.
		/// This is to reflect the inverse call order of mouse action trees (top first)
		void inspect(shared_ptr<IInspectable>);

		/// Refresh whatever is currently being inspected
		void refresh();

		/// Refresh if we're inspecting this
		void refresh(IInspectable *);

		void clear();
		shared_ptr<IInspectable> getTarget() const;

		void addToInspector(ElementPtr);

		ofxLiquidEvent<shared_ptr<IInspectable>> onTargetChange;
		ofxLiquidEvent<ElementPtr> onAddWidget;

		///this is actually triggered from the individual inspectors
		ofxLiquidEvent<InspectArguments> onClear;
	protected:
		weak_ptr<IInspectable> currentTarget;
		weak_ptr<IInspectable> inspectThisFrame;

		bool hasTarget = false;
		bool clearThisFrame = false;
		bool refreshThisFrame = false;
	};

	void inspect(shared_ptr<IInspectable>);

	bool isBeingInspected(shared_ptr<IInspectable>);
	bool isBeingInspected(IInspectable &);
	bool isBeingInspected(IInspectable *);

	void addToInspector(ofxCvGui::ElementPtr);

	void refreshInspector(IInspectable *);
}