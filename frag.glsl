#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform int resolution;

void main()
{
    vec2 mappedCoord = TexCoord * resolution;
    if (fract(mappedCoord.x + 0.1) < 0.2 || fract(mappedCoord.y + 0.1) < 0.2) {
        FragColor = vec4(0);
    } else {
        FragColor = texture(texture1, TexCoord);
    }
}