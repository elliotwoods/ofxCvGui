#version 150

uniform sampler2DRect tex0;

in vec2 texCoordVarying;
out vec4 outputColor;

uniform float minimum;
uniform float maximum;

void main() {

	vec4 inputColor = texture2DRect(tex0, texCoordVarying);
	outputColor = (inputColor - minimum) / (maximum - minimum);
}
