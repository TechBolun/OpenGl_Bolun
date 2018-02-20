#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 MVP_RenderToTexturePlane;

out vec2 UV;

void main() {
	gl_Position =  MVP_RenderToTexturePlane * vec4(Position, 1);
	UV = texCoord;

}