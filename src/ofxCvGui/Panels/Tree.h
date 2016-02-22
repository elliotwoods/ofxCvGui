#pragma once
#include "ofxCvGui/Panels/Scroll.h"

namespace ofxCvGui {
	namespace Panels {
		class Tree : public Panels::Scroll {
		public:
			class Branch : public Element {
			public:
				Branch();

				void addBranch(shared_ptr<Branch>);
				void removeBranch(shared_ptr<Branch>);
				void clear();

				void toggleShow();
				void collapse();
				void expand();

				float getDesiredHeight() const;
				ofxLiquidEvent<void> onChange;
			protected:
				vector<shared_ptr<Branch>> branches;
				bool collapsed = false;
				float desiredHeight = 25.0f;
				ElementPtr toggleButton;
			};

			Tree();
			shared_ptr<Branch> getRootBranch();
		protected:
			shared_ptr<Branch> rootBranch;
			bool needsArrange = false;
		};

		shared_ptr<Panels::Tree> makeTree(string caption = "");
	}
}