#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "types/ofBaseTypes.h"
#include "ofxAssets.h"

namespace ofxCvGui {
	namespace Panels {
		class Texture : public Panels::BaseImage {
		public:
			struct Style {
				float rangeMinimum = 0.0f;
				float rangeMaximum = 1.0f;
				shared_ptr<ofxAssets::Shader> shader = ofxAssets::Register::X().getShaderPointer("ofxCvGui::inputRange");
			};

			Texture(const ofTexture &);

			void setStyle(shared_ptr<Style>);
			shared_ptr<Style> getStyle() const;
			void setInputRange(float min, float max);
		protected:
			void drawImage(float width, float height) override;
			void drawInfo(DrawArguments &);
			float getImageWidth() const override;
			float getImageHeight() const override;

			const ofTexture & texture;
			shared_ptr<Style> style = nullptr;
		};

		shared_ptr<Panels::Texture> makeTexture(const ofTexture & asset, string caption = "");
	}
}