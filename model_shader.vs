#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform Material{
	vec4 aAmbient;
	vec4 aDiffuse;
	vec4 aSpecular;
};

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 Ambient;
out vec4 Diffuse;
out vec4 Specular;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    Position = vec3(model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;

    Ambient = aAmbient;
	Diffuse = aDiffuse;
	Specular = aSpecular;
}