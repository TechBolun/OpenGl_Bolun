#version 330

in vec2 UV;

out vec3 fragColor;

uniform sampler2D renderTexture;

void main() {
	
	fragColor = texture(renderTexture, UV).rgb;
	//fragColor = vec3(1.0f, 1.0f, 0.0f);
}