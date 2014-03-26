uniform mat4 g_mView;
uniform mat4 g_mProj;
uniform mat4 g_mViewProj;
uniform vec4 g_vAmbient;
uniform vec4 g_vLight;
uniform vec3 g_vEye;
uniform vec3 g_vEyeLook;

uniform vec3  posCamera;
uniform vec3  posLight;
uniform float rngLight;

varying vec3 N;
varying vec3 V;
varying vec3 L;
varying vec2 Texcoord;
varying vec4 Position;

varying float intensity;

varying vec4  ambient;
varying vec4  diffuse;
varying vec4  c_color;


const float shininess = 100.0;

void main(void)
{
   posCamera = g_vEye;
   posLight = vec3(g_vLight.x, g_vLight.y, g_vLight.z);
   rngLight = g_vLight.w;

   vec4 v = g_mView * gl_Vertex;
   vec3 n = vec3(g_mView * vec4(gl_Normal, 1));
   vec3 l = posLight - Position.xyz;
   vec3 c = posCamera - Position.xyz;

   N = normalize(n);
   L = normalize(l);
   V = normalize(c);
   intensity = dot(N, L) * shininess / length(l);
   intensity = max(intensity, 0.1);

   diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
   ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
   ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

   Position = v;

//   gl_Position = ftransform();
//   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;//g_mView * g_mProj;
   gl_Position = g_mProj * g_mView * gl_Vertex;//g_mView * g_mProj;
   Texcoord    = gl_MultiTexCoord0.xy;
}

//Fragment Shader
#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D txBase;
uniform vec3  posLight;
uniform float rngLight;

varying vec2 Texcoord;
varying vec4 Position;

varying vec3 N;
varying vec3 V;
varying vec3 L;
varying float intensity;

varying vec4  ambient;
varying vec4  diffuse;
varying vec4  c_color;



void main( void )
{
 vec4 color  = texture2D(txBase, Texcoord);
 vec4 color1 = ambient;
 float alpha = color.a;
 
 vec3  v_lp = posLight - Position.xyz;
 float rangle = rngLight;
// intensity *= rangle / length(posLight - Position.xyz);
 color *= intensity;
 color.a = alpha;              
 //gl_FragColor = color;
 gl_FragColor = vec4(1, 0, 0, 1);
}
