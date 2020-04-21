#version 450

layout(set = 0, binding = 1) uniform sampler2D txColor;
layout(set = 0, binding = 2) uniform sampler2D txNormal;
layout(set = 0, binding = 3) uniform sampler2D txSpecular;

layout(location = 0) out vec4 oColor;

void main()
{
  oColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
