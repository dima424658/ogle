#version 330 core

layout (location = 0) in vec2 aTexCoords;

out vec2 TexCoords;

uniform vec2 uScreenSize;
uniform vec2 uSpriteSize;

uniform mat4 model;
uniform mat4 view;
uniform mat4 orthographic;

void main()
{
    TexCoords = aTexCoords;
    vec4 position = orthographic * view * model * vec4(0.0, 0.0, 0.0, 1.0);
    position.x += ((aTexCoords.x * 2 - 1.0) / uScreenSize.x) * uSpriteSize.x;
    position.y += ((aTexCoords.y * 2 - 1.0) / uScreenSize.y) * uSpriteSize.y;

    gl_Position = position;
}