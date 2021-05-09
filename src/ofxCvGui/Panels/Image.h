#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "ofImage.h"

namespace ofxCvGui {
	namespace Panels {
		class OFXCVGUI_API_ENTRY Image : public Panels::BaseImage {
		public:
			Image(ofAbstractImage &);
            virtual ~Image();
			void setImage(ofAbstractImage &);
			ofAbstractImage * getImage();
		protected:
			void drawImage(float width, float height);
            void drawInfo(DrawArguments& arguments);
            float getImageWidth() const;
            float getImageHeight() const;
		private:
			ofAbstractImage * asset;
		};

		shared_ptr<Panels::Image> makeImage(ofAbstractImage& asset, string caption = "");
	}
}