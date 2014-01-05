uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vAmbient;
uniform vec4   g_vLight;
uniform vec3   g_vEye;
uniform vec3   g_vEyeDir;
uniform float  g_fTime;
uniform float  g_fRandom;


attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

varying vec3   normal;
varying vec3   ldir;
varying vec3   lpos;
varying vec3   eye;
varying vec2   texcoord;
varying vec4   vColor;


void main(void)
{
  vec4 pos = g_mTran * vec4(g_Vertex, 1.0);
  normal   = normalize(vec3(g_mTran * vec4(g_Normal, 1.0)));
  ldir     = vec3(g_vLight.xyz - pos.xyz);
  lpos     = g_vLight.xyz;
  eye      = g_vEye;

  gl_Position  = g_mProj * g_mView * pos;
  texcoord     = g_Texcoord;
  vColor       = g_Color;
}

//Fragment Shader
uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;

varying vec2   texcoord;
varying vec3   normal;
varying vec3   ldir;
varying vec3   lpos;
varying vec3   eye;
varying vec4   vColor;

void main( void )
{
  vec4 tcolor     = texture2D(g_txColor,    texcoord);
  vec4 tnormal    = texture2D(g_txNormal,   texcoord);
  vec4 tspecular  = texture2D(g_txSpecular, texcoord);
  vec4 spec       = vec4(0.0);

  vec3 b = normalize(tnormal.xyz * 2.0 - 1.0);
  vec3 n = normalize(normal + b);  //bump part
  vec3 l = normalize(ldir);
  vec3 e = normalize(eye);

  float shininess = 30.0;
  float intensity = max(dot(n,l), 0.1);

  tspecular = vec4(1.0);
  //specular
  {
    vec3 h = normalize(l + e);
    float kspec = max(dot(h, n), 0.0);
    spec = tspecular * pow(kspec, shininess);
  }

  gl_FragColor = intensity * tcolor * g_Color + spec;
}
