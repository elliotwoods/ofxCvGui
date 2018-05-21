#include "pch_ofxCvGui.h"

using namespace ofxCvGui;

namespace ofxCvGui {
	namespace Panels {
#pragma mark SharedViewServer
		//---------
		SharedViewServer::SharedViewServer() {
			this->onUpdate += [this](ofxCvGui::UpdateArguments &) {
				auto newFocus = this->focusThisFrame.lock();
				if (newFocus) {
					this->focus = newFocus;
					newFocus->getHostedView()->setBounds(this->getLocalBounds());
					this->focusThisFrame.reset();
				}

				auto focus = this->focus.lock();
				if (focus) {
					auto view = focus->getHostedView();
					if (view) {
						view->update();
					}
				}
			};

			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				auto focus = this->focus.lock();
				if (focus) {
					auto view = focus->getHostedView();
					if (view) {
						view->draw(args);
					}
				}
			};

			this->onMouse += [this](ofxCvGui::MouseArguments & args) {
				auto focus = this->focus.lock();
				if (focus) {
					auto view = focus->getHostedView();
					if (view) {
						view->mouseAction(args);
					}
				}
			};

			this->onKeyboard += [this](ofxCvGui::KeyboardArguments & args) {
				auto focus = this->focus.lock();
				if (focus) {
					auto view = focus->getHostedView();
					if (view) {
						view->keyboardAction(args);
					}
				}
			};

			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments & args) {
				auto focus = this->focus.lock();
				if (focus) {
					auto view = focus->getHostedView();
					if (view) {
						view->setBounds(this->getLocalBounds());
					}
				}
			};
		}

		//---------
		PanelPtr const SharedViewServer::findScreen(const ofVec2f & xy, ofRectangle & currentPanelBounds) {
			auto client = this->focus.lock();
			if (client) {
				return client->getHostedView()->findScreen(xy, currentPanelBounds);
			}
			else {
				return PanelPtr();
			}
		}

		//---------
		void SharedViewServer::setFocus(shared_ptr<SharedViewClient> client) {
			if (this->focusThisFrame.expired()) {
				//first per frame wins, as per inspector rules
				this->focusThisFrame = client;
			}
		}

		//---------
		void SharedViewServer::clearFocus() {
			auto focus = this->focus.lock();
			if (focus) {
				focus->takeBackView();
				this->focus.reset();
			}
		}

		//---------
		bool SharedViewServer::isFocused(shared_ptr<SharedViewClient> client) const {
			return this->focus.lock() == client;
		}

		//---------
		bool SharedViewServer::hasFocus() const {
			return (bool) this->focus.lock();
		}

#pragma mark SharedViewCLient
		//---------
		SharedViewClient::SharedViewClient(PanelPtr view, shared_ptr<SharedViewServer> server) {
			this->view = view;
			this->server = server;

			this->view->setScissorEnabled(false); //HACK
			this->setScissorEnabled(false); //HACK

			this->onUpdate += [this](ofxCvGui::UpdateArguments &) {
				if (!this->isFocused() && this->view) {
					this->view->update();
				}
			};

			this->onDraw += [this](ofxCvGui::DrawArguments & args) {
				if (!this->isFocused() && this->view) {
					DrawArguments nonConstArgs = args;
					this->view->draw(nonConstArgs);
				}
			};

			this->onMouse += [this](ofxCvGui::MouseArguments & args) {
				if (!this->isFocused() && this->view) {
					this->view->mouseAction(args);
				}
			};

			this->onKeyboard += [this](ofxCvGui::KeyboardArguments & args) {
				if (!this->isFocused() && this->view) {
					this->view->keyboardAction(args);
				}
			};

			this->onBoundsChange += [this](ofxCvGui::BoundsChangeArguments & args) {
				if (!this->isFocused() && this->view) {
					this->view->setBounds(args.localBounds);
				}
			};
		}

		//---------
		PanelPtr const SharedViewClient::findScreen(const ofVec2f & xy, ofRectangle & currentPanelBounds) {
			if (this->view) {
				return this->view->findScreen(xy, currentPanelBounds);
			} else {
				return PanelPtr();
			}
		}

		//---------
		void SharedViewClient::focusView() {
			auto server = this->server.lock();
			if (server) {
				server->setFocus(shared_from_this());
			}
		}

		//---------
		bool SharedViewClient::isFocused() {
			auto server = this->server.lock();
			if (server) {
				return server->isFocused(shared_from_this());
			}
			else {
				return false;
			}
		}

		//---------
		PanelPtr SharedViewClient::getHostedView() {
			return this->view;
		}

		//---------
		void SharedViewClient::takeBackView() {
			if (this->view) {
				this->view->setBounds(this->getLocalBounds());
			}
		}
	}
}