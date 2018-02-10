# version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

uniform mat4 MVP_tranform;
uniform mat4 MV_tranform;
uniform mat4 MW_tranform;

out vec3 worldPos;
out vec3 worldNormal;
out vec4 Color;

void main() {

	gl_Position = MVP_tranform * vec4(Position.x, Position.y, Position.z , 1.0);
	//Color = vec4(1.0, 1.0, 1.0, 1.0);
	Color = MV_tranform * vec4(Normal, 1.0);
	worldNormal = (MV_tranform * vec4(Normal, 1.0)).xyz;
	worldPos = (MW_tranform * vec4(Position, 1.0)).xyz;

}