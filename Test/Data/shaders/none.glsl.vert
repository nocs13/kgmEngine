#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0)  mat4   g_mView; 
layout(binding = 1)  mat4   g_mProj;           
layout(binding = 2)  mat4   g_mTran;           
layout(binding = 3)  vec4   g_vColor;          
layout(binding = 4)  vec4   g_vSpecular;       
layout(binding = 5)  vec4   g_vLight;          
layout(binding = 6)  vec4   g_vLightColor;     
layout(binding = 7)  vec4   g_vLightDirection; 
layout(binding = 8)  vec4   g_vClipPlane;      
layout(binding = 9)  vec3   g_vUp;             
layout(binding = 10) vec3   g_vEye;            
layout(binding = 11) vec3   g_vLook;           
layout(binding = 12) float  g_fTime;           
layout(binding = 13) float  g_fShine;          
layout(binding = 14) float  g_fRandom;         
layout(binding = 15) float  g_fAmbient;        
layout(binding = 16) float  g_fLightPower;     
layout(binding = 17) int    g_iClipping;

layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_UV;

void main() 
{
    gl_Position = ubo.g_mProj * ubo.g_mView * ubo.g_mTran * vec4(a_Vertex, 1.0);
}

