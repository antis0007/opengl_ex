#version 330 core
out vec4 frag_color;
in vec4 vertex_color;
uniform vec4 uni_color;

void main()
{
    frag_color = uni_color;
}
