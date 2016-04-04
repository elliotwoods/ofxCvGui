#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "types/ofBaseTypes.h"

namespace ofxCvGui {
	namespace Panels {
		class Draws : public Panels::BaseImage {
		public:
			Draws();
			Draws(ofBaseDraws &);

			void setDrawObject(ofBaseDraws &);
			void clearDrawObject();
			ofBaseDraws * getDrawObject() const;
		protected:
			void drawImage(float width, float height);
            float getImageWidth() const;
            float getImageHeight() const;
		private:
			ofBaseDraws * drawObject;
		};

		shared_ptr<Panels::Draws> makeBaseDraws(ofBaseDraws& asset, string caption = "");
	}
}