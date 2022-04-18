#include "pch_ofxCvGui.h"
#include "SubMenu.h"
#include "ofxCvGui/InspectController.h"

namespace ofxCvGui {
	class TemporaryInspectable : public IInspectable {
	public:
		TemporaryInspectable(const std::function<void(InspectArguments&)>& inspectFunction)
		{
			this->onPopulateInspector += [inspectFunction](ofxCvGui::InspectArguments& args) {
				inspectFunction(args);
			};
		}
	protected:
	};

	namespace Widgets {
		//--------
		SubMenuInspectable::SubMenuInspectable(const std::string& caption
			, std::shared_ptr<IInspectable> inspectable
			, bool ownInspectable
			, char hotKey)
			: Button(caption, hotKey)
		{
			this->inspectable = inspectable;

			if (ownInspectable) {
				this->onHit += [this]() {
					ofxCvGui::inspectWithOwnership(this->inspectable);
				};
			}
			else {
				this->onHit += [this]() {
					ofxCvGui::inspect(this->inspectable);
				};
			}
			

			// Draw an arrow icon
			this->onDraw += [](ofxCvGui::DrawArguments& args) {
				auto bounds = args.localBounds;
				bounds.width = 30;
				bounds.x = args.localBounds.getRight() - bounds.width;
				Utils::drawGlyph(u8"\uf105", bounds);
			};

			// Little taller than usual
			this->setHeight(80.0f);
		}

		//--------
		std::shared_ptr<IInspectable>
			SubMenuInspectable::getInspectable()
		{
			return this->inspectable;
		}

		//--------
		SubMenuFunctional::SubMenuFunctional(const std::string& caption
			, const std::function<void(InspectArguments&)>& inspectFunction
			, char hotKey)
			: SubMenuInspectable(caption, make_shared<TemporaryInspectable>(inspectFunction), true, hotKey)
		{
		}

		//--------
		SubMenuFunctional::SubMenuFunctional(ofParameterGroup& parameterGroup
			, char hotKey)
			: SubMenuFunctional(parameterGroup.getName(), [&parameterGroup](InspectArguments& args) {
			auto inspector = args.inspector;
			inspector->addParameterGroup(parameterGroup);
				}, hotKey)
		{
		}
	}
}