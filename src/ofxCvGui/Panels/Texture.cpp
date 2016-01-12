#include "Texture.h"
#include "ofxAssets.h"

namespace ofxCvGui {
	namespace Panels {
		//----------
		bool Texture::Style::operator==(const Style & other) const {
			if (other.rangeMaximum != this->rangeMaximum) {
				return false;
			}
			if (other.rangeMinimum != this->rangeMinimum) {
				return false;
			}
			return true;
		}

		//----------
		Texture::Texture(const ofTexture & texture) :
			texture(texture) {
			this->onDraw += [this](DrawArguments & args) {
				this->drawInfo(args);
			};
		}

		//----------
		void Texture::setStyle(const Style & style) {
			this->style = style;
		}

		//----------
		const Texture::Style & Texture::getStyle() const {
			return this->style;
		}

		//----------
		void Texture::drawImage(float width, float height) {
			if (this->texture.isAllocated()) {
				auto isDefaultStyle = this->style == Style();

				if (!isDefaultStyle) {
					auto & shader = ofxAssets::shader("ofxCvGui::inputRange");
					shader.begin();
					shader.setUniform1f("minimum", this->style.rangeMinimum);
					shader.setUniform1f("maximum", this->style.rangeMaximum);

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
	}
}