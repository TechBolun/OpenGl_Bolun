#version 330

layout (location = 0) in vec3 position;

out vec3 texCoord;

uniform mat4 MVP_skybox;

void main()
{
    gl_Position =  MVP_skybox * vec4(position, 1.0);  
    texCoord = position;
}