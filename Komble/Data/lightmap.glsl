uniform mat4 g_mView;
uniform mat4 g_mProj;
uniform mat4 g_mViewProj;
uniform vec4 g_vAmbient;

varying vec2 Texcoord0;
varying vec2 Texcoord1;


void main(void)
{
   gl_Position = ftransform();
   Texcoord0    = gl_MultiTexCoord0.xy;
   Texcoord1    = gl_MultiTexCoord1.xy;
}
//Fragment Shader
uniform sampler2D txBase;
uniform sampler2D txLight;

varying vec2 Texcoord0;
varying vec2 Texcoord1;


void main( void )
{
 vec4 color0 = texture2D(txBase, Texcoord0);
 vec4 color1 = texture2D(txLight, Texcoord1);
 vec3 col3 = color0.xyz * color1.xyz; 
// col3 = vec3(1, 0, 0);
 gl_FragColor = vec4(col3.xyz, 1.0);
// gl_FragColor = vec4(color0.xyz, 1.0);
// gl_FragColor = vec4(color1.xyz, 1.0);
}