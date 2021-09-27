#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

//uniform sampler2D texture1;
uniform float borderWidth;
uniform vec3 color;

void main()
{
    if (TexCoord.x > 1 - borderWidth || TexCoord.x < borderWidth || TexCoord.y > 1 - borderWidth || TexCoord.y < borderWidth) {
        FragColor = vec4(0,0,0,0);
    } else {
        FragColor = vec4(color, 1);
    }
//    vec2 mappedCoord = TexCoord * resolution;
//    if (fract(mappedCoord.x + 0.1) < 0.2 || fract(mappedCoord.y + 0.1) < 0.2) {
//        FragColor = vec4(0);
//    } else {
//        FragColor = texture(texture1, TexCoord);
//    }
}