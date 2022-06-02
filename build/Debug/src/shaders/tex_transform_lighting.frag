#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform sampler2D ourTexture;
uniform vec4 uni_color;
void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient + diffuse) * texture(ourTexture, TexCoord);
    FragColor = vec4(result, 1.0);

    //Hackjob RGBA for tinting
    FragColor[0] += uni_color[0]*uni_color[3];
    FragColor[1] += uni_color[1]*uni_color[3];
    FragColor[2] += uni_color[2]*uni_color[3];

}
