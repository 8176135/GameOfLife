#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 tint;
uniform vec3 baseColor;

void main()
{
    FragColor = vec4(tint, 1) * vec4(baseColor, 1);
}