#version 450

layout(binding = 1) uniform sampler2D texColor;

layout(location = 0) in vec4  color;
layout(location = 1) in vec2 uv0;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = color * texture(texColor, uv0);
}
