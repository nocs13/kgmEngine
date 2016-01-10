uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform float  g_fAmbient;

varying vec3   V;
varying vec2   Texcoord;
varying float  I;

attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

void main(void)
{
   V = vec4(g_mTran * vec4(g_Vertex, 1.0)).xyz;
   I = g_fAmbient;

   gl_Position   = g_mProj * g_mView * vec4(V, 1.0);
   Texcoord      = g_Texcoord;
}

//Fragment Shader
#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D g_txColor;
uniform float     g_fAlpha;

varying vec3   V;
varying vec2   Texcoord;
varying float  I;

void main( void )
{
 vec4 color = texture2D(g_txColor, Texcoord);

 vec4  col = vec4(color.xyz * I, color.w * g_fAlpha);

 gl_FragColor = col;
}
