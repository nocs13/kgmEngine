uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vAmbient;
uniform vec4   g_vLight;
uniform vec3   g_vEye;
uniform vec3   g_vEyeDir;
uniform float  g_fTime;
uniform float  g_fRandom;

uniform vec4   g_vLights[31];
uniform int    g_iLights;

varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec2   Texcoord;
varying float  I;

attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

void main(void)
{
   mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                     g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                     g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);
   V = vec4(g_mTran * vec4(g_Vertex, 1.0)).xyz;
   N = normalize(mRot * g_Normal);
   I = g_vLight.w;
   L = g_vLight.xyz;

   gl_Position   = g_mProj * g_mView * vec4(V, 1.0);
   Texcoord      = g_Texcoord;
}

//Fragment Shader
#ifdef GL_ES
precision lowp float;
#endif

uniform vec4 g_vLights[31];
uniform int  g_iLights;

uniform sampler2D g_txColor;

varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec2   Texcoord;
varying float  I;

void main( void )
{
 vec4 color = texture2D(g_txColor,    Texcoord);

 vec3 LN = normalize(L - V);

 float distance = length(L - V);
 float intensity = I * dot(N, LN) / (1.0 + distance);

 for(int i = 0; i < g_iLights; i++)
 {
   vec3  ln = normalize(g_vLights[i].xyz - V);
   float dn = length(g_vLights[i].xyz - V);
   intensity += (10 * dot(N, ln) / (1.0 + dn));
 }

 vec4  col = vec4(color.xyz * intensity, color.w);

 col = clamp(col, 0.1, 0.95);

 gl_FragColor = col;
}
