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
uniform sampler2DShadow txDepth;

varying vec4 q;



void main( void )
{
 vec3 scoord = 0.5 * q.xyz / q.w + 0.5;
// scoord.xy = clamp(scoord.xy, 0.01, 0.99);
 if((scoord.x < 0.0) || (scoord.x > 1.0))
  discard;
 if((scoord.y < 0.0) || (scoord.y > 1.0))
  discard;
  
 scoord    = clamp(scoord, 0.0, 1.0);
 float shadow = shadow2D(txDepth, scoord).r;
 vec3 color = vec3(1.0, 1.0, 1.0);
// if(shadow < q.z){
// if(shadow < 0.0000001){
//	if(shadow < 0.5){
//		shadow = 0.5;
//	}
// }else{
//  discard;
//  shadow = 1.0;
// }
 if(shadow > 0.9)
  discard;
 shadow = 0.1;  
 color *= shadow;
 gl_FragColor = vec4(color, 0.5);
}
