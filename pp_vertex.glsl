#version 410 core
layout (location = 0) in vec3 modelVerts;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

//uniform mat4 position;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
    // The quad that is used to render to the screen is only half the dimensions of the screen
    gl_Position = vec4(modelVerts * 2, 1.0f);
    TexCoord = aTexCoord;
}