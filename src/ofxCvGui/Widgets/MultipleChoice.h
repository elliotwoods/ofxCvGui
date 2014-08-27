#pragma once
#include "Toggle.h"

namespace ofxCvGui {
	namespace Widgets {
		class MultipleChoice : public Element {
		public:
			MultipleChoice();
			void addChoice(string);
			void removeChoice(string);
			void clearChoices();

			void select(int);
			void select(string);

			int getSelectionIndex() const;
			string getSelection() const;

			ofxLiquidEvent<int> selectionChange;
		protected:
			vector<shared_ptr<Toggle>> choices;
		};
	}
}