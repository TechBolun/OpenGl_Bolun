# version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 MVP_tranform;
uniform mat4 MV_tranform;
uniform mat4 MW_tranform;

uniform vec3 lightPosition_Bolun;

out vec3 worldPos;
out vec3 viewNormal;
out vec4 Color;
out vec2 UV;

out vec3 worldNormal;

void main() {

	gl_Position = MVP_tranform * vec4(Position.x, Position.y, Position.z , 1.0);

	Color = MV_tranform * vec4(Normal, 1.0);


	viewNormal = (MV_tranform * vec4(Normal, 1.0)).xyz;
	worldPos = (MW_tranform * vec4(Position, 1.0)).xyz;

	worldNormal = mat3(transpose(inverse(MW_tranform))) * Normal;

	UV = texCoord;

}