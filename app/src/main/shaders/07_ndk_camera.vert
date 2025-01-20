#version 460
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoordinates;

layout (location = 0) out vec2 outTextureCoordinates;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    outTextureCoordinates = inTextureCoordinates;
}