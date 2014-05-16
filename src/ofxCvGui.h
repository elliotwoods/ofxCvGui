#pragma once

#include "ofxCvGui/Controller.h"
#include "ofxCvGui/Builder.h"

#include "ofxCvGui/Utils/Button.h"

#include "ofxCvGui/Widgets/IInspectable.h"
#include "ofxCvGui/Widgets/Title.h"
#include "ofxCvGui/Widgets/Spacer.h"
#include "ofxCvGui/Widgets/Slider.h"
#include "ofxCvGui/Widgets/Toggle.h"
#include "ofxCvGui/Widgets/Button.h"
#include "ofxCvGui/Widgets/LiveValue.h"

void inspect(ofxCvGui::Widgets::IInspectable & target) {
	ofxCvGui::Panels::Inspector::setSelection(target);
}