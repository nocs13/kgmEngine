uniform mat4 g_mView;
uniform mat4 g_mProj;
uniform mat4 g_mTran;
uniform mat4 g_mLight;

uniform vec3 eye_position;
uniform vec3 eye_direction;
uniform vec4 light;

varying vec3 N;
varying vec3 V;
varying vec3 L;
varying vec2 Texcoord0;
varying vec2 Texcoord1;

varying vec4 q;


void main(void)
{
   q = g_mLight * gl_Vertex;
   gl_Position = g_mProj * g_mView * gl_Vertex;
}

//Fragment Shader
uniform sampler2D txBase;
uniform sampler2D txBump;
uniform sampler2D txLight;
uniform sampler2DShadow txDepth;

varying vec2 Texcoord0;
varying vec2 Texcoord1;

varying vec3 N;
varying vec3 V;
varying vec3 L;

varying vec4 q;



void main( void )
{
 vec4 color0 = texture2D(txBase, Texcoord0);
 vec4 color1 = texture2D(txLight, Texcoord1);
 vec4 color2 = texture2D(txBump, Texcoord0);
 vec3 scoord = 0.5 * q.xyz / q.w + 0.5;
 scoord.xy = clamp(scoord.xy, 0.01, 0.99);
 float shadow = shadow2D(txDepth, scoord).r;
 
 float inten = dot(color2.xyz, L);
   float strength = 1.0;
   vec3  final = vec3(1, 1, 1);
   N = normalize(N);
   L = normalize(L);
   strength = dot(N, L);
   if(strength > 0.5){
    strength = 1.0;
   }else if(strength >= 0.0){
    strength = 0.5;
   }else if(strength < 0.0){
    strength = 0.5;
   }

//   if((q.x > -0.99 && q.x < 0.99) && (q.y > -0.99 && q.y < 0.99)){
//   if(q.x > -1.0){
//    if(q.x < 1.0){
//	 strength *= shadow;
//	}
//   }
//   if((q.x > -1.0) && (q.x < 1.0)&& (q.y > -1.0) && (q.y < 1.0))
   if(shadow < q.z){
	if(shadow < 0.5)
		shadow = 0.5;
//	strength *= shadow;
   }
   
   gl_FragColor = vec4(color0.xyz * color1.xyz * strength, color0.w);
//   gl_FragColor = vec4(color0.xyz * color1.xyz * color2.xyz * strength, color0.w);
//   gl_FragColor = vec4(color0.xyz * color1.xyz * strength * inten, color0.w);
}
