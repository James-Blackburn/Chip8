#version 460 core

layout (location = 0) uniform sampler2D quadTexture;

in vec2 TexCoords;
out vec4 FragColour;

void main() { 
    vec2 flippedTexCoords = vec2(TexCoords.x, 1.0 - TexCoords.y);
    FragColour = texture(quadTexture, flippedTexCoords);
}