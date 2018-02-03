# version 330

layout (location = 0) in vec3 Position;

uniform mat4 MVP_tranform;

out vec4 Color;

void main() {

	gl_Position = MVP_tranform * vec4(Position.x, Position.y, Position.z , 1.0);
	//Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
	Color = vec4(1.0, 1.0, 1.0, 1.0);

}