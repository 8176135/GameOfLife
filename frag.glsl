#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord;

//uniform sampler2D texture1;
uniform float borderWidth;
uniform vec3 color;

void main()
{
    if (TexCoord.x > 1 - borderWidth || TexCoord.x < borderWidth || TexCoord.y > 1 - borderWidth || TexCoord.y < borderWidth) {
        FragColor = vec4(0, 0, 0, 0);
    } else {
        FragColor = vec4(color, 1);
    }

//    float brightness = dot(FragColor.rgb, vec3(1 / 3));
    float brightness = FragColor.r; // Grayscale anyway
    if (brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    //    vec2 mappedCoord = TexCoord * resolution;
    //    if (fract(mappedCoord.x + 0.1) < 0.2 || fract(mappedCoord.y + 0.1) < 0.2) {
    //        FragColor = vec4(0);
    //    } else {
    //        FragColor = texture(texture1, TexCoord);
    //    }
}