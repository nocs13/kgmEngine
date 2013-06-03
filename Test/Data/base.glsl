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

varying out vec3 N;
varying out vec3 V;
varying out vec3 L;
varying out vec2 Texcoord;

void main(void)
{
   vec4 pos_final = g_mProj * g_mView * g_mTran * gl_Vertex;
   N  = normalize(vec3(g_mTran * vec4(gl_Normal, 0.0)));
   L  = normalize(g_vLight.xyz - vec4(g_mTran * gl_Vertex).xyz);
   V  = pos_final.xyz;
   gl_Position  = pos_final;
   Texcoord     = gl_MultiTexCoord0.xy;
}

//Fragment Shader
uniform sampler2D txColor;
uniform sampler2D txNormal;
uniform sampler2D txSpecular;
uniform vec3      g_vEyeDir;

varying in vec2   Texcoord;
varying in vec3   N;
varying in vec3   V;
varying in vec3   L;

void main( void )
{
 vec4 color     = texture2D(txColor,    Texcoord);
 vec4 normal    = texture2D(txNormal,   Texcoord);
 vec4 specular  = texture2D(txSpecular, Texcoord);

 normal.xyz = normalize(N + normal.xyz);

 float intensity   = 1.0;
       intensity = clamp(dot(normal.xyz, L), 0.2, 1.0);
 //intensity        += dot(normal, g_vEyeDir);
 gl_FragColor = vec4(clamp(color.x  * intensity, 0.0, 1.0),
                     clamp(color.y  * intensity, 0.0, 1.0),
                     clamp(color.z  * intensity, 0.0, 1.0),
                     color.w);
 //gl_FragColor = color;
 //gl_FragColor = vec4(1, 0, 0, 1);
}
