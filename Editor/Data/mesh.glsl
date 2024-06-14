uniform mat4 g_mView;
uniform mat4 g_mProj;
uniform mat4 g_mTran;

uniform vec3 eye_position;
uniform vec3 eye_direction;
uniform vec3 light_position;

varying vec3 N;
varying vec3 V;
varying vec3 L;
varying vec2 Texcoord;
varying vec2 Texcoord1;

void main(void)
{
   mat3 mnor = mat3(g_mTran._11, g_mTran._12, g_mTran._13, 
					g_mTran._21, g_mTran._22, g_mTran._23, 
					g_mTran._31, g_mTran._32, g_mTran._33);
   vec4 pos = g_mTran * gl_Vertex;
//   vec3 nor = gl_NormalMatrix * gl_Normal;
   vec3 nor = mnor * gl_Normal;	
   nor = normalize(nor);
   if(length(nor) < 0.1)
    nor = vec3(1, 0, 0);
//   nor = eye_direction;
   vec3 l = light_position - pos.xyz;
   vec3 c = eye_position - pos.xyz;
   N = normalize(nor);
   L = normalize(l);
   V = normalize(c);
//   N = vec3(1, 0, 0);
//   L = vec3(0, 1, 0);
//   V = vec3(0, 0, 1);
//   vec3 normal = gl_Normal;
//   vec3 binormal = cross(vec3(1, 0, 0), normal);
//   vec3 tangent  = cross(normal, binormal);
//   mat3 t;
//   t[0] = tangent;
//   t[1] = binormal;
//   t[2] = normal;

   gl_Position = g_mProj * g_mView * g_mTran * gl_Vertex;
   Texcoord    = gl_MultiTexCoord0.xy;
   Texcoord1   = gl_MultiTexCoord1.xy;
}
//Fragment Shader
uniform sampler2D txBase;
uniform sampler2D txBump;
uniform sampler2D txLight;
uniform sampler2DShadow txDepth;

varying vec2 Texcoord;
varying vec2 Texcoord1;

varying vec3 N;
varying vec3 V;
varying vec3 L;



void main( void )
{
   vec4 color  = texture2D(txBase, Texcoord);
   vec4 color1 = texture2D(txLight, Texcoord1);
 
   float strength = 1.0;
   vec3  final = vec3(1, 1, 1);
   N = normalize(N);
   L = normalize(L);
   V = normalize(V);
   strength = dot(N, L);
   if(strength > 0.5){
    strength = 1.0;
   }else if(strength >= 0.0){
    strength = 0.5;
   }else if(strength < 0.0){
    strength = 0.3;
   }
   strength = 1.0;
//   color.xyz = 1.0;
   gl_FragColor = vec4(color.xyz * color1.xyz * strength, color.w);
//   gl_FragColor = vec4(color.xyz * strength, color.w);
//   gl_FragColor = vec4(color.xyz, color.w);
}