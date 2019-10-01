#version 450
layout(binding = 1) uniform sampler2D texColor;
layout(binding = 2) uniform sampler2D texNormal;
layout(binding = 3) uniform sampler2D texSpecular;
layout(binding = 4) uniform sampler2D texFlexible;

layout(location = 0) in vec4  color;

layout(location = 1) in vec4  position;
layout(location = 2) in float clipping;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = color;
}
