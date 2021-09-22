#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
//uniform int resolution;

void main()
{
    FragColor = vec4(0.5, 0, 0, 0);
}