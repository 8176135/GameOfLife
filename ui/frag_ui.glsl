#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 tint;

void main()
{
    FragColor = vec4(tint, 1) * vec4(1, 0, 0, 1) ;
}