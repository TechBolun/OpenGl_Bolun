# version 330

in vec3 textureDir;

uniform samplerCube cubemap;

void main()
{
    color = texture(cubemap, textureDir);
}