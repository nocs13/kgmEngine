uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform mat3   g_mNorm;
uniform vec4   g_vAmbient;
uniform vec3   g_vEye;
uniform vec3   g_vEyeDir;
uniform float  g_fTime;
uniform float  g_fRandom;

uniform vec4   g_vLights[12];
uniform int    g_iLights;

attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

varying vec3   position;
varying vec3   normal;
varying vec3   eye;
varying vec2   texcoord;

varying vec4   lights[12];

void main(void)
{
  vec4 pos = g_mTran * vec4(g_Vertex, 1.0);

  position = vec3(pos);
  normal   = normalize(g_Normal);

  eye          = vec3(-pos.xyz);

  for(int i = 0; i < g_iLights; i++)
  {
    vec3 ldir = pos.xyz - g_vLights[i].xyz;
    lights[i].w = g_vLights[i].w / length(ldir);
    lights[i].xyz = normalize(ldir);
  }

  gl_Position  = g_mProj * g_mView * pos;
  texcoord     = g_Texcoord;
}

//Fragment Shader
uniform mat4      g_mTran;
uniform vec4      g_vLights[12];
uniform float     g_fRandom;
uniform int       g_iLights;

uniform vec4      g_vTEST;

uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;

varying vec2      texcoord;
varying vec3      position;
varying vec3      normal;
varying vec3      eye;

varying vec4      lights[12];


void main( void )
{
  vec4 color     = texture2D(g_txColor,    texcoord);
  vec4 tnormal   = texture2D(g_txNormal,   texcoord);
  vec4 specular  = texture2D(g_txSpecular, texcoord);

  vec3 b = normalize(tnormal.xyz * 2.0 - 1.0);
  vec3 n = normalize(tnormal.xyz);
  vec3 e = normalize(eye);

  float shininess = 0.1;
  float intensity = 0.0;

  float   I = clamp(10.0 * g_fRandom, 0.0, 11.0);

  for(int i = 0; i < g_iLights; i++)
  {
    vec3   l = (lights[i].xyz);

    intensity += (lights[i].w) * max(dot(normal,l), 0.01);
  }

  gl_FragColor = intensity * vec4(1.0);//color;
}
