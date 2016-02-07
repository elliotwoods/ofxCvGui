#version 150

uniform sampler2DRect tex0;
uniform float minimum;
uniform float maximum;

in vec2 texCoordVarying;
out vec4 outputColor;

void main() {
	vec4 inputColor = texture(tex0, texCoordVarying);
	outputColor = (inputColor - minimum) / (maximum - minimum);
}
