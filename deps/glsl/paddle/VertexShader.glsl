#version 330 core

layout (location = 0) in vec2 iPos;

uniform mat4 uTranslation;
uniform mat4 uRotation;
uniform mat4 uScaling;

void main() {
    gl_Position = uTranslation * uScaling * uRotation * vec4(iPos, 0, 1);
}