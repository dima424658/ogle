#version 330 core
layout (location = 0) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aTexCoords.x * 2 - 1.0, aTexCoords.y * 2 - 1.0, 0.0, 1.0);
}