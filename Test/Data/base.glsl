uniform mat4  g_mView;
uniform mat4  g_mProj;
uniform mat4  g_mTran;
uniform vec4  g_vAmbient;
uniform vec4  g_vLight;
uniform vec3  g_vEye;
uniform vec3  g_vEyeLook;

varying vec3  posCamera;
varying vec3  posLight;
varying float rngLight;

varying vec3 N;
varying vec3 V;
varying vec3 L;
varying vec2 Texcoord;
varying vec4 Position;

varying float intensity;

varying vec4  ambient;
varying vec4  diffuse;
varying vec4  c_color;


const float shininess = 1.0;

void main(void)
{
   N  = normalize(vec3(g_mTran * vec4(gl_Normal,0.0)));
   L  = normalize(g_vLight.xyz - vec4(g_mTran * gl_Vertex).xyz);
   gl_Position = g_mProj * g_mView * g_mTran * gl_Vertex;
   Texcoord    = gl_MultiTexCoord0.xy;
}

//Fragment Shader
uniform sampler2D txDiffuse;
uniform sampler2D txNormal;
uniform sampler2D txSpecular;

varying vec3      posLight;
varying float     rngLight;

varying vec2      Texcoord;
varying vec4      Position;

varying vec3      N;
varying vec3      V;
varying vec3      L;
varying float     intensity;

varying vec4  ambient;
varying vec4  diffuse;
varying vec4  c_color;

void main( void )
{
 vec4 color   = texture2D(txDiffuse, Texcoord);
 float intensity   = max(dot(N, L), 0.2);
 gl_FragColor = color * intensity;
 //gl_FragColor = vec4(1, 0, 0, 1);
}
