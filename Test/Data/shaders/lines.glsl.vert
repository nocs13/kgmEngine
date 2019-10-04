#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject 
{
 mat4   g_mView;           
 mat4   g_mProj;           
 mat4   g_mTran;           
 vec4   g_vColor;          
 vec4   g_vSpecular;       
 vec4   g_vLight;          
 vec4   g_vLightColor;     
 vec4   g_vLightDirection; 
 vec4   g_vClipPlane;      
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

layout(push_constant) uniform ConstBlock
{
  mat4   model;
  vec4   color;
  vec4   specular;
} cb;

layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in uint a_Color;

layout(location = 0) out vec4  color;

void main() 
{
  vec4 col =  unpackUnorm4x8(a_Color);

  color = col;//cb.color;

  vec4 position = cb.model * vec4(a_Vertex, 1.0);

  vec4 pos = ubo.g_mProj * ubo.g_mView * position;

  pos.y *= -1;

  gl_Position = pos;
}
