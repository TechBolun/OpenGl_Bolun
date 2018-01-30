# version 330

layout (location = 0) in vec3 Position;

uniform mat4 MVP_tranform;

void main() {
	gl_Position = vec4(0.05 *Position.x, 0.05 *Position.y, 0.05 *Position.z , 1.0);
}