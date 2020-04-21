#version 450

layout(set = 0, binding = 1) uniform sampler2D txColor;
layout(set = 0, binding = 2) uniform sampler2D txNormal;
layout(set = 0, binding = 3) uniform sampler2D txSpecular;

layout(location = 0) in vec4  color;

layout(location = 1) in vec4  position;
layout(location = 2) in float clipping;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = color;
}
