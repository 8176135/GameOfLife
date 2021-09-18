#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform int resolution;

void main()
{
    if (TexCoord.x > 0.9 || TexCoord.x < 0.1 || TexCoord.y > 0.9 || TexCoord.y < 0.1) {
        FragColor = vec4(0,0,0,0);
    } else {
        FragColor = vec4(1,1,1,1);
    }
//    vec2 mappedCoord = TexCoord * resolution;
//    if (fract(mappedCoord.x + 0.1) < 0.2 || fract(mappedCoord.y + 0.1) < 0.2) {
//        FragColor = vec4(0);
//    } else {
//        FragColor = texture(texture1, TexCoord);
//    }
}