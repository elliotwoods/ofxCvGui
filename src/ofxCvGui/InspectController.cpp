#include "InspectController.h"

namespace ofxCvGui {
#pragma mark IInspectable
	//----------
	bool IInspectable::isBeingInspected() const {
		return InspectController::X().getTarget().get() == this;
	}

#pragma mark InspectController
	//----------
	InspectController * InspectController::singleton = nullptr;

	//----------
	InspectController::InspectController() {
		this->hasTarget = false;
		this->clearThisFrame = false;
	}

	//----------
	InspectController & InspectController::X() {
		if (!InspectController::singleton) {
			InspectController::singleton = new InspectController();
		}
		return * InspectController::singleton;
	}

	//----------
	void InspectController::update() {
		bool needsToNotifyListeners = false;

		//if this frame we got a command to clear
		if (this->clearThisFrame) {
			this->clearThisFrame = false;
			this->currentTarget.reset();
			needsToNotifyListeners = true;
		}

		//if a selection was made this frame then take it
		if (this->inspectThisFrame) {
			if (this->currentTarget.lock() != this->inspectThisFrame) {
				this->currentTarget = this->inspectThisFrame;
				this->inspectThisFrame.reset();
				needsToNotifyListeners = true;
			}
			this->inspectThisFrame.reset();
		}

		//if our target has been deleted elsewhere, let's drop our attention to it before something weird happens
		if (this->currentTarget.expired() && this->hasTarget) {
			needsToNotifyListeners = true;
		}

		//notify inspectors of any change in target
		if (needsToNotifyListeners) {
			auto target = this->currentTarget.lock();
			this->hasTarget = (bool)target;
			this->onTargetChange(target);
		}
	}

	//----------
	void InspectController::inspect(shared_ptr<IInspectable> target) {
		if (!this->inspectThisFrame) {
			//only set if nothing else has been set this frame
			this->inspectThisFrame = target;
		}
	}

	//----------
	void InspectController::clear() {
		this->clearThisFrame = true;
	}
	
	//----------
	shared_ptr<IInspectable> InspectController::getTarget() const {
		return this->currentTarget.lock();
	}

	//----------
	void InspectController::addToInspector(ElementPtr element) {
		this->onAddWidget.notifyListeners(element);
	}

#pragma mark Global
	//-----------
	void inspect(shared_ptr<IInspectable> target) {
		InspectController::X().inspect(target);
	}

	//-----------
	bool isBeingInspected(shared_ptr<IInspectable> target) {
		return target == InspectController::X().getTarget();
	}

	//-----------
	bool isBeingInspected(IInspectable & target) {
		auto currentInspectTarget = InspectController::X().getTarget();
		return &target == currentInspectTarget.get();
	}

	//-----------
	void addToInspector(ofxCvGui::ElementPtr element) {
		InspectController::X().addToInspector(element);
	}
}