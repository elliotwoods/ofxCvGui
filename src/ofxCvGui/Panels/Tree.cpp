#include "pch_ofxCvGui.h"


namespace ofxCvGui {
	namespace Panels {
#pragma mark Tree::Branch
		//----------
		Tree::Branch::Branch() {
			//this->setCachedView(true);

			this->onDraw += [this](DrawArguments & args) {
				ofPushStyle();
				{
					ofDrawBitmapString(this->getCaption(), 20, 15);

					if (this->branches.empty()) {
						ofSetColor(100);
						ofSetLineWidth(1.0f);
						ofDrawLine(5, 10, 15, 10);
					}
				}
				ofPopStyle();
			};

			//arrange all the elements (we do this even if they're disabled, ready for later)
			this->onBoundsChange += [this](BoundsChangeArguments & args) {
				float x = 20.0f;
				float width = args.localBounds.width - x;
				float y = 20.0f;

				for (auto branch : this->branches) {
					ofRectangle bounds;
					bounds.x = x;
					bounds.y = y; 
					bounds.width = width;
					bounds.height = branch->getDesiredHeight();
					branch->setBounds(bounds);

					y += bounds.height;
				}
			};

			this->onMouse += [this](MouseArguments & args) {
				args.takeMousePress(this);
				if (args.isDoubleClicked(this)) {
					this->toggleShow();
				}
			};

			//update our height ready to be arranged
			this->onChange += [this]() {
				auto height = 0.0f;

				height += 20.0f; // header area
				if (!this->collapsed) {
					for (auto branch : this->branches) {
						height += branch->getDesiredHeight();
					}
				}
				height += 5.0f; // gutter
				this->desiredHeight = height; // we cache this value to be applied on next update (onChange will go down the tree and trigger a flag in Tree view)
				
				this->toggleButton->setEnabled(!this->branches.empty()); //only show toggle button if we have subbranches
				this->markViewDirty();
			};

			this->toggleButton = make_shared<Element>();
			this->toggleButton->setBounds(ofRectangle(0, 0, 20, 20));
			this->toggleButton->onDraw += [this](DrawArguments & args) {
				ofPushStyle();
				{
					ofNoFill();
					ofSetLineWidth(1.0f);
					ofDrawRectangle(ofRectangle(5, 5, 10, 10));

					if (this->collapsed) {
						//draw a plus
						ofDrawLine(7, 10, 13, 10);
						ofDrawLine(10, 7, 10, 13);
					}
					else {
						//draw a minus
						ofDrawLine(7, 10, 13, 10);
					}
				}
				ofPopStyle();
			};
			this->toggleButton->onMouse += [this](MouseArguments & args) {
				if (args.takeMousePress(this)) {
					this->toggleShow();
				}
			};
			this->toggleButton->addListenersToParent(this);
		}

		//----------
		void Tree::Branch::addBranch(shared_ptr<Branch> branch) {
			this->branches.push_back(branch);
			branch->addListenersToParent(this);
			branch->onChange.addListener([this]() {
				this->onChange.notifyListeners();
			}, this);
			branch->onChange.notifyListeners(); //trigger the chain of changes from the top
		}

		//----------
		void Tree::Branch::removeBranch(shared_ptr<Branch> branch) {
			for (auto it = this->branches.begin(); it != this->branches.end(); it++) {
				if (*it == branch) {
					this->branches.erase(it);
					branch->onChange.removeListeners(this);
					this->onChange.notifyListeners();
					break;
				}
			}
		}
		
		//----------
		void Tree::Branch::clear() {
			for (auto branch : this->branches) {
				branch->removeListenersFromParent(this);
				branch->clear();
			}
			this->branches.clear();
			this->onChange.notifyListeners();
		}

		//----------
		void Tree::Branch::toggleShow() {
			if (this->collapsed) {
				this->expand();
			}
			else {
				this->collapse();
			}
		}

		//----------
		void Tree::Branch::collapse() {
			this->collapsed = true;
			for (auto branch : this->branches) {
				branch->setEnabled(false);
			}
			this->onChange.notifyListeners();
		}

		//----------
		void Tree::Branch::expand() {
			this->collapsed = false;
			for (auto branch : this->branches) {
				branch->setEnabled(true);
			}
			this->onChange.notifyListeners();
		}

		//----------
		float Tree::Branch::getDesiredHeight() const {
			return this->desiredHeight;
		}

#pragma mark Tree
		//----------
		Tree::Tree() {
			this->rootBranch = make_shared<Branch>();
			this->add(this->rootBranch);
			this->rootBranch->onChange += [this]() {
				this->needsArrange = true;
			};
			this->onUpdate += [this](UpdateArguments &) {
				if (this->needsArrange) {
					this->rootBranch->setHeight(rootBranch->getDesiredHeight());
					this->arrange();
					this->needsArrange = false;
				}
			};
		}

		//----------
		shared_ptr<Tree::Branch> Tree::getRootBranch() {
			return this->rootBranch;
		}

		//----------
		shared_ptr<Panels::Tree> makeTree(string caption) {
			auto newPanel = make_shared<Panels::Tree>();
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}