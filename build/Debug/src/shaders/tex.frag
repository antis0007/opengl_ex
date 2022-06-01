#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 uni_color;
void main()
{
    FragColor = texture(ourTexture, TexCoord);
    //Hackjob RGBA
    FragColor[0] += uni_color[0]*uni_color[3];
    FragColor[1] += uni_color[1]*uni_color[3];
    FragColor[2] += uni_color[2]*uni_color[3];

}
