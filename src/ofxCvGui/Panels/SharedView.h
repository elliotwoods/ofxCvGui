#pragma once

#include "ofxCvGui/Panels/Base.h"

namespace ofxCvGui {
	namespace Panels {
		/// The view host abstracts the view away from the element
		///	where it is being stored. This is in order to allow for
		/// the view to move over to the summary pane when the node
		/// is selected.

		class SharedViewClient;

		class SharedViewServer : public ofxCvGui::Panels::Base {
		public:
			SharedViewServer();
			ofxCvGui::PanelPtr const findScreen(const ofVec2f & xy, ofRectangle & currentPanelBounds) override;
			void setFocus(shared_ptr<SharedViewClient>);
			void clearFocus();
			bool isFocused(shared_ptr<SharedViewClient>) const;
			bool hasFocus() const;
		protected:
			weak_ptr<SharedViewClient> focus;
			weak_ptr<SharedViewClient> focusThisFrame;
		};

		class SharedViewClient : public ofxCvGui::Panels::Base, public std::enable_shared_from_this<SharedViewClient> {
		public:
			SharedViewClient(ofxCvGui::PanelPtr view, shared_ptr<SharedViewServer>);
			ofxCvGui::PanelPtr const findScreen(const ofVec2f & xy, ofRectangle & currentPanelBounds) override;
			void focusView();
			bool isFocused();
			PanelPtr getHostedView();

			void takeBackView();
		protected:
			ofxCvGui::PanelPtr view;
			weak_ptr<SharedViewServer> server;
		};
	}
}