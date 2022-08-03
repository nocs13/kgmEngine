#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_LIGHTS 16

struct Light
{
    vec4 pos;
    vec4 dir;
};

layout(set = 0, binding = 0) uniform  UBO
{
 mat4   g_mView;
 mat4   g_mProj;
 mat4   g_mTran;
 vec4   g_vColor;
 vec4   g_vSpecular;
 vec4   g_vClipPlane;

 Light  g_sLights[MAX_LIGHTS];

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

layout(set = 0, binding = 1) uniform sampler2D txColor;

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

layout(location = 0) in Data  data;

layout(location = 0) out vec4 fragColor;

void main()
{
  vec4 tcolor = texture(txColor,    data.uv);

  vec4 fcolor = vec4(0, 0, 0, 0);

  fcolor.xyz  = data.color.xyz * tcolor.xyz;
  fcolor.w    = data.color.w * tcolor.w;

  fragColor = fcolor;
}
