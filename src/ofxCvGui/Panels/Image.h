#pragma once
#include "ofxCvGui/Panels/BaseImage.h"
#include "ofImage.h"

namespace ofxCvGui {
	namespace Panels {
		class Image : public Panels::BaseImage {
		public:
			Image(ofImage &);
            virtual ~Image();
		protected:
			void drawImage(float width, float height);
            void drawInfo(DrawArguments& arguments);
            float getImageWidth() const;
            float getImageHeight() const;
		private:
			ofImage & asset;
		};
	}
}