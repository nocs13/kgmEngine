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
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_UV;

layout(location = 0) out vec4  color;
layout(location = 1) out vec4  position;
layout(location = 2) out float clipping;

void main() 
{
    color = ubo.g_vColor * a_Color;

    gl_Position = ubo.g_mProj * ubo.g_mView * ubo.g_mTran * vec4(a_Vertex, 1.0);
}

