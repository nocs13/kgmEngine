#version 120
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

attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec2 g_Texcoord;

void main(void)
{
   vec3 vvv = g_Vertex;
   V = vec4(g_mTran * gl_Vertex).xyz;
//   V = vec4(g_mTran * vec4(g_Vertex, 1.0)).xyz;
   N  = normalize(vec3(g_mTran * vec4(gl_Normal, 0.0)));
   L  = normalize(g_vLight.xyz - V);

   gl_Position  = g_mProj * g_mView * vec4(V, 1.0);
   Texcoord     = gl_MultiTexCoord0.xy;
}

//Fragment Shader
uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;
uniform vec3      g_vEyeDir;

varying vec2   Texcoord;
varying vec3   N;
varying vec3   V;
varying vec3   L;

void main( void )
{
 vec4 color     = texture2D(g_txColor,    Texcoord);
 vec4 normal    = texture2D(g_txNormal,   Texcoord);
 vec4 specular  = texture2D(g_txSpecular, Texcoord);

 normal.xyz = normal.xyz + N;
 normal.xyz = normalize(normal.xyz);

 float intensity  = 1.0;
       intensity  = clamp(2.0 * dot(normal.xyz, L), 0.1, 1.0);
//       intensity += dot(normal.xyz, g_vEyeDir);
       intensity  = clamp(intensity, 0.1, 1.0);

 vec3  col = (color.xyz + specular.xyz) * intensity;

 col = clamp(col, 0.0, 1.0);

 gl_FragColor = vec4(col.x, col.y, col.z, color.w);
// gl_FragColor = color;
// gl_FragColor = specular;
// gl_FragColor = vec4(1, 0, 0, 1);
}
