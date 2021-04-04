#version 120
#define JOINTS  35
#define EPSILON 0.001


uniform mat4 g_mView;
uniform mat4 g_mProj;
uniform mat4 g_mTran;
uniform mat4 g_mJoints[JOINTS];


uniform vec3 eye_position;
uniform vec3 eye_direction;
uniform vec3 light_position;

varying vec3 N;
varying vec3 V;
varying vec3 L;
varying vec2 Texcoord;

varying float tmp;

void main(void)
{
    //mat3 mnor = mat3(g_mTran._11, g_mTran._12, g_mTran._13,
    //                 g_mTran._21, g_mTran._22, g_mTran._23,
    //                 g_mTran._31, g_mTran._32, g_mTran._33);
    //build skin
    int   index  = 10;
    float weight = 1.0;

    vec4 epos = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 spos = gl_Vertex;

    weight = gl_MultiTexCoord1.x;
    index  = int(gl_MultiTexCoord2.x);
    epos   = g_mJoints[index] * spos;// * weight;

/*    weight = gl_MultiTexCoord1.y;
    index  = int(gl_MultiTexCoord2.y);
    epos += g_mJoints[index] * spos * weight;

    weight = gl_MultiTexCoord1.z;
    index  = int(gl_MultiTexCoord2.z);
    epos += g_mJoints[index] * spos * weight;

    weight = gl_MultiTexCoord1.w;
    index  = int(gl_MultiTexCoord2.w);
    epos += g_mJoints[index] * spos * weight;    */

    epos.w = 1.0;
    ////////////

    vec4 pos = g_mTran * epos;
    vec3 nor = vec4( g_mTran * vec4(gl_Normal, 1.0)).xyz;

    nor = normalize(nor);

    if(length(nor) < 0.1)
        nor = vec3(1, 0, 0);

    vec3 l = light_position - pos.xyz;
    vec3 c = eye_position   - pos.xyz;

    N = normalize(nor);
    L = normalize(l);
    V = normalize(c);

    gl_FrontColor = vec4(1.0, .0, .0, 1.0);
    gl_Position = g_mProj * g_mView * pos;
//    gl_Position = g_mProj * g_mView * g_mTran * epos;
//    gl_Position = g_mProj * g_mView * g_mTran * gl_Vertex;
    Texcoord    = gl_MultiTexCoord1.xy;
}

//Fragment Shader
uniform sampler2D txBase;

varying vec2 Texcoord;
varying vec3 N;
varying vec3 V;
varying vec3 L;
varying float tmp;

void main( void )
{
    vec4  color  = texture2D(txBase, Texcoord);
    float strength = 1.0;
    vec3  final = vec3(1, 1, 1);
    vec3  n = normalize(N);
    vec3  l = normalize(L);
    vec3  v = normalize(V);
    strength = dot(N, L);

    if(strength > 0.5){
        strength = 1.0;
    }else if(strength >= 0.0){
        strength = 0.5;
    }else if(strength < 0.0){
        strength = 0.3;
    }

    gl_FragColor = vec4(color.xyz * strength, color.w);
}
