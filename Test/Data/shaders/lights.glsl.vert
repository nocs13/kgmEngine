#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_LIGHTS 8

struct InLight
{
  vec4 position;
  vec4 direction;
  vec4 color;
};

layout(binding = 0) uniform UniformBufferObject 
{
 mat4   g_mView;           
 mat4   g_mProj;           
 mat4   g_mTran;           
 vec4   g_vColor;          
 vec4   g_vSpecular;       
 vec4   g_vClipPlane;      

 InLight g_vLights[MAX_LIGHTS];

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

layout(push_constant) uniform ConstBlock
{
  mat4   model;
  vec4   color;
  vec4   specular;
} cb;

struct Light
{
  vec3  pos;
  vec3  dir;
  vec3  col;
  float angle;
  float power;
};

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
  float lcount;

  Light lights[MAX_LIGHTS];
};

layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in float a_Color;
layout(location = 3) in vec2 a_UV;

layout(location = 0) out Data data;

void main() 
{
  mat3  mRot = mat3(cb.model);

  vec4 pos = cb.model * vec4(a_Vertex, 1.0);

  data.position  = cb.model * vec4(a_Vertex, 1.0);

  data.posinview = ubo.g_mView * data.position;

  data.nor = mRot * a_Normal;

  data.eye = ubo.g_vEye;

  data.shine = ubo.g_fShine;

  data.uv = a_UV;

  //data.color = unpackUnorm4x8(uint(a_Color));
  data.color = ubo.g_vColor;

  pos =  ubo.g_mProj * ubo.g_mView * pos;

  data.lcount = float(ubo.g_iLights);

  for (int i = 0; i < ubo.g_iLights; i++)
  {
    data.lights[i].pos = ubo.g_vLights[i].position.xyz;
    data.lights[i].col = ubo.g_vLights[i].color.rgb;
    data.lights[i].dir = ubo.g_vLights[i].direction.xyz;

    data.lights[i].power = ubo.g_vLights[i].position.w;
    data.lights[i].angle = ubo.g_vLights[i].direction.w;
  }

  pos.y = -pos.y;

  gl_Position = pos;
}
