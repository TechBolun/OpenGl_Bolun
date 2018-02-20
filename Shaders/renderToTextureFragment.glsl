#version 330

in vec2 UV;

out vec3 fragColor;

uniform sampler2D texture;

void main() {
	
	fragColor = texture(texture, UV).rgb;

}