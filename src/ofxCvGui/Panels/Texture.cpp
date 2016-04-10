#include "Texture.h"
#include "ofxAssets.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		Texture::Texture(const ofTexture & texture) :
			texture(texture) {
			this->onDraw += [this](DrawArguments & args) {
				this->drawInfo(args);
			};
		}

		//----------
		void Texture::setStyle(shared_ptr<Texture::Style> style) {
			this->style = style;
		}

		//----------
		shared_ptr<Texture::Style> Texture::getStyle() const {
			return this->style;
		}

		//----------
		void Texture::setInputRange(float min, float max) {
			auto style = make_shared<Style>();
			style->rangeMinimum = min;
			style->rangeMaximum = max;
			this->setStyle(style);
		}

		//----------
		void Texture::drawImage(float width, float height) {
			if (this->texture.isAllocated()) {
				if (this->style) {
					auto & shader = this->style->shader->get();

					shader.begin();
					if(this->style->shader == ofxAssets::Register::X().getShaderPointer("ofxCvGui::inputRange")) {
						shader.setUniform1f("minimum", this->style->rangeMinimum);
						shader.setUniform1f("maximum", this->style->rangeMaximum);
						shader.setUniformTexture("tex0", this->texture, 0);
					}

					this->texture.draw(0, 0, width, height);

					shader.end();
				}
				else {
					this->texture.draw(0, 0, width, height);
				}
			}
		}

		//----------
		void Texture::drawInfo(DrawArguments & args) {
			if (!args.chromeEnabled)
				return;

			stringstream ss;
			if (this->texture.isAllocated()) {
				ss << this->texture.getWidth() << "x" << this->texture.getHeight() << ", Format = " << this->texture.getTextureData().glInternalFormat;
			}
			else {
				ss << "Unallocated";
			}

			Utils::drawText(ss.str(), 20, this->getHeight() - 30, true, 20);
		}

		//----------
		float Texture::getImageWidth() const {
			return this->texture.getWidth();
		}

		//----------
		float Texture::getImageHeight() const {
			return this->texture.getHeight();
		}

		//----------
		shared_ptr<Panels::Texture> makeTexture(const ofTexture & asset, string caption) {
			auto newPanel = make_shared<Panels::Texture>(asset);
			OFXCVGUI_LABEL_PANEL_AND_RETURN
		}
	}
}