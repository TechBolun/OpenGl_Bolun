#version 330

layout (location = 0) in vec3 Position;

out vec3 texCoord;

uniform mat4 MVP_skybox;

void main() {

    gl_Position =  MVP_skybox * vec4(Position, 1.0);
	
    texCoord = Position;
}