#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "types/ofBaseTypes.h"

namespace ofxCvGui {
	namespace Panels {
		class Texture : public Panels::BaseImage {
		public:
			struct Style {
				bool operator==(const Style &) const;
				float rangeMinimum = 0.0f;
				float rangeMaximum = 1.0f;
			};

			Texture(const ofTexture &);

			void setStyle(const Style &);
			const Style & getStyle() const;
		protected:
			void drawImage(float width, float height) override;
			void drawInfo(DrawArguments &);
			float getImageWidth() const override;
			float getImageHeight() const override;

			const ofTexture & texture;
			Style style;
		};
	}
}