#version 460 core

layout (location = 0) uniform sampler2D quadTexture;

in vec2 TexCoords;
out vec4 FragColour;

void main() { 
    FragColour = texture(quadTexture, TexCoords);
}