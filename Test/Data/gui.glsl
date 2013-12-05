#version 120
uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;

varying vec2   Texcoord;
varying vec4   PxColor;

attribute vec3 g_Vertex;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

void main(void)
{
   gl_Position  = g_mProj * g_mView * g_mTran * gl_Vertex;
   Texcoord     = gl_MultiTexCoord0.xy;
   PxColor      = gl_Color;
}

//Fragment Shader
uniform sampler2D g_txColor;

varying vec2   Texcoord;
varying vec4   PxColor;

void main( void )
{
 vec4 color     = texture2D(g_txColor,    Texcoord);

 gl_FragColor = color * PxColor;
}
