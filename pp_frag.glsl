#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D screenTexture;

const float offset = 1.0 / 600.0;

void main()
{
    vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
//    vec2 mappedCoord = TexCoord * resolution;
//    if (fract(mappedCoord.x + 0.1) < 0.2 || fract(mappedCoord.y + 0.1) < 0.2) {
//        FragColor = vec4(0);
//    } else {
//        FragColor = texture(texture1, TexCoord);
//    }
}