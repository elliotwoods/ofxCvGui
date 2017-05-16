#include "InspectController.h"

//----------
OFXSINGLETON_DEFINE(ofxCvGui::InspectController);

namespace ofxCvGui {
#pragma mark IInspectable
	//----------
	bool IInspectable::isBeingInspected() const {
		return InspectController::X().getTarget().get() == this;
	}

#pragma mark InspectController
	//----------
	InspectController::InspectController() {
		
	}

	//----------
	void InspectController::update() {
		bool needsToNotifyListeners = false;

		//if this frame we got a command to clear
		if (this->clearThisFrame) {
			this->currentTarget.reset();
			needsToNotifyListeners = true;
			this->clearThisFrame = false;
		}

		//if a selection was made this frame then take it
		auto inspectThisFrame = this->inspectThisFrame.lock();
		if (inspectThisFrame) {
			if (this->currentTarget.lock() != inspectThisFrame) {
				this->currentTarget = this->inspectThisFrame;
				needsToNotifyListeners = true;
			}
			this->inspectThisFrame.reset();
		}

		//if a refresh is activated
		if (this->refreshThisFrame) {
			if (this->currentTarget.lock()) {
				//if we have a valid target
				needsToNotifyListeners = true;
			}
 			this->refreshThisFrame = false;
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
		//only set if nothing else has been set this frame
		if (!this->inspectThisFrame.lock()) {
			if (target) {
				this->inspectThisFrame = target;
			}
			else {
				this->clear();
			}
		}
	}

	//----------
	void InspectController::refresh() {
		this->refreshThisFrame = true;
	}

	//----------
	void InspectController::refresh(IInspectable * target) {
		if (this->currentTarget.lock().get() == target) {
			this->refresh();
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

	//----------
	void InspectController::maximise() {
		this->onMaximise.notifyListeners();
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
		return isBeingInspected(&target);
	}

	//-----------
	bool isBeingInspected(IInspectable * target) {
		auto currentInspectTarget = InspectController::X().getTarget();
		return target == currentInspectTarget.get();
	}

	//-----------
	void addToInspector(ofxCvGui::ElementPtr element) {
		InspectController::X().addToInspector(element);
	}

	//-----------
	void refreshInspector(IInspectable * target) {
		InspectController::X().refresh(target);
	}
}