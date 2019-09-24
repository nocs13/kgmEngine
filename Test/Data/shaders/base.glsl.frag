#version 450

layout(location = 0) in vec4  color;

layout(location = 1) in vec4  position;
layout(location = 2) in float clipping;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = color;
}
