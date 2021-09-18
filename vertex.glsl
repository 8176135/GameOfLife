#version 410 core
layout (location = 0) in vec3 modelVerts;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 position;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * position * vec4(modelVerts, 1.0f);
    TexCoord = aTexCoord;
}