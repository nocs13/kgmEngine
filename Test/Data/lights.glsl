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

attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

varying vec3   position;
varying vec3   normal;
varying vec3   eye;
varying vec2   texcoord;

void main(void)
{
  vec4 pos = g_mTran * vec4(g_Vertex, 1.0);

  position = vec3(pos);
  normal   = normalize(g_Normal);

  eye          = vec3(-pos.xyz);

  gl_Position  = g_mProj * g_mView * pos;
  texcoord     = g_Texcoord;
}

//Fragment Shader
uniform mat4      g_mTran;
uniform vec4      g_vLights[12];

uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;

varying vec2      texcoord;
varying vec3      position;
varying vec3      normal;
varying vec3      eye;

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

  for(int i = 0; i < 12; i++)
  {
    vec3   l = position - vec3(vec4(g_vLights[i].xy, 5, 1.0));
    float  d = length(l);

    l = normalize(l);
    intensity += 0.99 * (g_vLights[i].w / d) * max(dot(normal,l), 0.01);
  }

  gl_FragColor = intensity * color;
  //gl_FragColor = vec4(g_vLights[3].w, 0, 0, 1);
}
