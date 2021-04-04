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
layout(location = 2) in vec2 a_UV;

layout(location = 0) out vec4 color;
layout(location = 1) out vec2 uv0;

void main() 
{
  vec4 v_col = vec4((a_Color & 0x000000FF) >> 0, (a_Color & 0x0000FF00) >> 8,
                    (a_Color & 0x00FF0000) >> 16, (a_Color & 0xFF000000)>> 24);

  //color = ubo.g_vColor;
  color = cb.color;
  //color = vec4(v_col.g);

  uv0   = a_UV;
  
  vec4 pos = ubo.g_mProj * vec4(a_Vertex, 1.0);
  
  pos.y *= -1;

  gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}
