
#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 MVP_tranform;

out vec2 UV;

void main() {
	UV = texCoord;
	
	gl_Position =  MVP_tranform * vec4(Position, 1);
	
}