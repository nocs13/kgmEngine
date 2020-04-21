#version 450

#define MAX_LIGHTS 8

layout(binding = 0) uniform UniformBufferObject
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
 int    g_iLights;
} ubo;

layout(location = 0) in vec2 vPos;

void main()
{
  gl_Position = vec4(vPos.x, vPos.y, 0.0f, 1.0f);

  gl_Position.y *= -1;
}
