uniform mat4 g_mView;
uniform mat4 g_mProj;
uniform mat4 g_mWorld;
uniform mat4 g_mViewProj;
uniform vec4 g_vAmbient;
uniform vec4 g_vLight;
uniform vec3 g_vEye;
uniform vec3 g_vEyeLook;

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
  vec4 vtmp;
   posCamera = g_vEye;
   vtmp = g_mView * vec4(g_vLight.x, g_vLight.y, g_vLight.z, 0);
   posLight = vtmp.xyz;
   //posLight = vec3(g_vLight.x, g_vLight.y, g_vLight.z);
   rngLight = g_vLight.w;

   vec4 v = g_mView * gl_Vertex;

   Position = gl_Vertex; //v;

   vec3 n = vec3(g_mView * vec4(gl_Normal, 1));
   vec3 l = posLight - Position.xyz;
   vec3 c = posCamera - Position.xyz;

   N = normalize(n);
   L = normalize(l);
   V = normalize(c);
   intensity = dot(N, L); // * shininess / length(l);
   intensity = max(intensity, 0.1);

   //diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
   //ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
   //ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;


   gl_Position = g_mProj * g_mView * gl_Vertex;
   Texcoord    = gl_MultiTexCoord0.xy;
}

//Fragment Shader
varying vec3  posLight;
varying float rngLight;

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
 vec4 color  = vec4(1, 1, 1, 1);
 //vec4 color1 = ambient;
 float alpha = color.a;
 float inten = intensity;

 vec3  v_lp = posLight - Position.xyz;
 float rangle = rngLight;
 inten *= 30.0 / length(v_lp);
 color *= inten;
 color.a = 1.0;
 gl_FragColor = color;
 //gl_FragColor = vec4(1, 0, 0, 1);
}
