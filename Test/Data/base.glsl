uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vAmbient;
uniform vec4   g_vLight;
uniform vec3   g_vEye;
uniform vec3   g_vEyeDir;
uniform float  g_fTime;
uniform float  g_fRandom;

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

uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;
uniform vec3      g_vEyeDir;

varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec2   Texcoord;
varying float  I;

void main( void )
{
 vec4 color     = texture2D(g_txColor,    Texcoord);
 vec4 normal    = texture2D(g_txNormal,   Texcoord);
 vec4 specular  = texture2D(g_txSpecular, Texcoord);

 normal = (2.0 * normal) - 1.0;
 normal.xyz = normal.xyz + N;
 normal.xyz = normalize(normal.xyz);

 vec3 vN = normalize(normal.xyz);
 vec3 vL = normalize(L - V);
 vec3 Y  = normalize(-g_vEyeDir);

 float distance = length(L - V);
 float intensity  = 1.0;
       intensity  = max(dot(normal.xyz, normalize(vL)), 0.0) / (1.0 + 0.1 * distance);
       intensity  = clamp(intensity, 0.2, 0.7);
 vec3  reflection = normalize(normal.xyz * 2.0 * intensity - vL);
 //float ispecular  = pow(clamp(dot(reflection, Y), 0, 1), 10);
 float ispecular  = pow(clamp(dot(vL, vN), 0.0, 1.0), 100.0);

 vec4  col = vec4(color.xyz * intensity, color.w) + vec4(specular.xyz * ispecular, 0);

 col = clamp(col, 0.1, 1.0);

 gl_FragColor = col;
}
