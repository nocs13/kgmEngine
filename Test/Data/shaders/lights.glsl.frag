#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_LIGHTS 8

layout(set = 0, binding = 0) uniform  UBO
{
 mat4   g_mView;
 mat4   g_mProj;
 mat4   g_mTran;
 vec4   g_vColor;
 vec4   g_vSpecular;
 vec4   g_vClipPlane;

 vec4   g_vLightPos[MAX_LIGHTS];
 vec4   g_vLightDir[MAX_LIGHTS];
 vec4   g_vLightCol[MAX_LIGHTS];

 vec3   g_vUp;
 vec3   g_vEye;
 vec3   g_vLook;
 float  g_fTime;
 float  g_fShine;
 float  g_fRandom;
 float  g_fAmbient;
 float  g_fLightPower;
 int    g_iClipping;
} ubo;

// layout(offset = 240) vec4   g_vLightPos[MAX_LIGHTS];

layout(set = 0, binding = 1) uniform sampler2D txColor;
layout(set = 0, binding = 2) uniform sampler2D txNormal;
layout(set = 0, binding = 3) uniform sampler2D txSpecular;
layout(set = 0, binding = 4) uniform sampler2D txFlexible;

struct Data
{
  vec4 color;
  vec4 position;
  vec4 posinview;

  vec3 nor;
  vec3 eye;
  vec3 eyedir;
  vec2 uv;

  float shine;
};

struct Light
{
  vec3 pos;
  vec3 dir;
  vec3 col;

  float intensity;
  float angle;
};

layout(location = 0) in Data  data;

layout(location = 0) out vec4 fragColor;

void main()
{
  vec4 tcolor     = texture(txColor,    data.uv);
  vec4 tnormal    = texture(txNormal,   data.uv);
  vec4 tspecular  = texture(txSpecular, data.uv);
  vec4 spec       = vec4(0.0);

  vec3 b = normalize(tnormal.xyz * 2.0 - 1.0);
  vec3 n = normalize(b + normalize(data.nor));
  vec3 e = normalize(data.eye);

  vec3 lcol = vec3(1.0);

  float intensity = 0.0;

  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    Light  l;

    l.pos = ubo.g_vLightPos[0].xyz;
    l.dir = ubo.g_vLightDir[0].xyz;
    l.col = ubo.g_vLightCol[0].rgb;

    l.intensity = ubo.g_vLightPos[0].w;
    l.angle     = ubo.g_vLightDir[0].w;

    if (length(l.dir) < 0.95)
    {
      l.dir = normalize(l.pos - data.position.xyz);
    }

    intensity += (dot(n, l.dir));
    lcol *= l.col;
  }

  intensity = clamp(intensity, 0.1, 1.0);

  vec4 fcolor = vec4(0, 0, 0, 0);
  fcolor.xyz  = data.color.xyz * lcol.xyz * intensity;
  fcolor.w    = data.color.w * tcolor.w;

  fragColor = fcolor;
}
