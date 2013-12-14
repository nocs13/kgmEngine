#define MAX_LIGHTS  32
uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vAmbient;
uniform vec3   g_vEye;
uniform vec3   g_vEyeDir;
uniform float  g_fTime;
uniform float  g_fRandom;

uniform vec4   g_vLights[MAX_LIGHTS];


attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

varying vec3   normal;
varying vec3   eye;
varying vec2   texcoord;
varying vec3   ldirs[MAX_LIGHTS];
varying float  linfl[MAX_LIGHTS];


void main(void)
{
  vec4 pos = g_mTran * vec4(g_Vertex, 1.0);
  normal   = normalize(vec3(g_mTran * vec4(g_Normal, 1.0)));

  for(int i = 0; i < MAX_LIGHTS; i++)
  {
    ldirs[i] = vec3(g_vLights[i].xyz - pos.xyz);
    linfl[i] = g_vLights[i].w / length(ldirs[i]);
    ldirs[i] = normalize(ldirs[i]);
  }
  eye      = vec3(-pos.xyz);

  gl_Position  = g_mProj * g_mView * pos;
  texcoord     = g_Texcoord;
}

//Fragment Shader
#define MAX_LIGHTS  32
uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;

varying vec2   texcoord;
varying vec3   normal;
varying vec3   eye;
varying vec3   ldirs[MAX_LIGHTS];
varying float  linfl[MAX_LIGHTS];


void main( void )
{
  vec4 color     = texture2D(g_txColor,    texcoord);
  vec4 tnormal   = texture2D(g_txNormal,   texcoord);
  vec4 specular  = texture2D(g_txSpecular, texcoord);

  vec3 b = normalize(tnormal.xyz * 2.0 - 1.0);
  vec3 n = normalize(tnormal.xyz);
  //vec3 l = normalize(ldir);
  vec3 e = normalize(eye);

  float shininess = 0.1;
  float intensity = max(dot(n,l), 0.1);

  for(int i = 0; i < MAX_LIGHTS; i++)
  {

  }

  gl_FragColor = intensity * color;
}
