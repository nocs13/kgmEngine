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

layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in uint a_Color;
layout(location = 2) in vec2 a_UV;

layout(location = 0) out vec4  color;

void main() 
{
  color = ubo.g_vColor;
  
  vec4 acolor;
  
  acolor.r = ((a_Color & 0x000000ff) >> 0)  / 255.0;
  acolor.g = ((a_Color & 0x0000ff00) >> 8)  / 255.0;
  acolor.b = ((a_Color & 0x00ff0000) >> 16) / 255.0;
  acolor.a = ((a_Color & 0xff000000) >> 24) / 255.0;
  
  color = vec4(acolor.b);
  
  vec4 pos = ubo.g_mProj * vec4(a_Vertex, 1.0);
  
  pos.y *= -1;

  gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    //gl_Position = ubo.g_mProj * ubo.g_mView * ubo.g_mTran * vec4(a_Vertex, 1.0);
}

