#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

in vec4 Ambient;
in vec4 Diffuse;
in vec4 Specular;

uniform sampler2D texture_diffuse;
uniform vec3 viewPos;

void main()
{
    vec3 color = vec3(0.5f, 0.5f, 0.5f);
    vec3 lightPos = vec3(-2.0f, 2.0f, 0.0f);

    vec3 ambient = Diffuse.rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);

    vec3 diffuse = max(dot(norm, lightDir), 0.0) * Diffuse.rgb;
    vec3 diffuse2 = max(dot(norm, vec3(0.5f, -0.5f, 0.0f)), 0.0) * Diffuse.rgb;

    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
    vec3 specular = spec * Specular.rgb;

    color = color * (diffuse + diffuse2 + ambient + specular);

    FragColor = vec4(color, 1.0f);
}