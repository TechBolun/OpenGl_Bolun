# version 330

layout (location = 0) in vec3 Position;

uniform mat4 MVP_tranform;

void main() {

	gl_Position = MVP_tranform * vec4(Position.x, Position.y, Position.z , 1.0);

}