# version 330

//uniform vec3 diffuse_Bolun;
//uniform vec3 specular_Bolun;
uniform vec3 ambientColor_Bolun;
uniform vec3 lightPosition_Bolun;
uniform vec3 cameraPosition_Bolun;

uniform sampler2D diffuse_Bolun;
uniform sampler2D specular_Bolun;

uniform samplerCube skybox;

uniform mat4 MW_tranform;
uniform mat4 MV_tranform;

in vec4 Color;
in vec3 worldPos;
in vec3 worldNormal;
in vec3 viewNormal;
in vec2 UV;

out vec4 fragColor;

void main() {
	
	vec3 worldLightDir = normalize(lightPosition_Bolun - worldPos); //works
	vec3 viewDir = normalize(cameraPosition_Bolun - worldPos); // works
	vec3 half = normalize(viewDir + worldLightDir); //works

	float brightness = clamp(dot(half, normalize(viewNormal)), 0, 1);
	float cosThetaLN = clamp(dot(worldLightDir, normalize(viewNormal)), 0, 1);

	brightness = cosThetaLN != 0.0 ? brightness : 0.0;

	vec3 diffuse = texture(diffuse_Bolun, UV).rgb;

	diffuse = diffuse * cosThetaLN;

	vec3 specular = texture(specular_Bolun, UV).rgb;

	specular = specular * pow(brightness, 25);

	fragColor = vec4(diffuse + specular, 1.0f);

	//fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	viewDir = normalize(worldPos - cameraPosition_Bolun);
	vec3 reflectDir = reflect(viewDir, normalize(worldNormal));

	fragColor = texture(skybox, reflectDir);

}