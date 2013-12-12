//#version 120
uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vAmbient;
uniform vec4   g_vLight;
uniform vec3   g_vEye;
uniform vec3   g_vEyeDir;
uniform float  g_fTime;
uniform float  g_fRandom;

varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec2   Texcoord;
varying float  Ldist;

attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

varying vec3   normal;
varying vec3   ldir;
varying vec3   eye;


void main(void)
{
   //V = vec4(g_mTran * vec4(g_Vertex, 1.0)).xyz;
   //N = normalize(vec3(g_mTran * vec4(g_Normal, 1.0)));
   //L = normalize(g_vLight.xyz - V);
   //Ldist = distance(g_vLight.xyz, V);

   //gl_Position  = g_mProj * g_mView * vec4(V, 1.0);
   //Texcoord     = g_Texcoord;

    vec4 pos = vec4(0);//g_mTran * vec4(g_Vertex, 1.0);
    //normal = normalize(vec3(g_mTran * vec4(g_Normal, 1.0)));
    ldir = vec3(g_vLight.xyz - pos.xyz);
    eye = vec3(-pos.xyz);
    gl_Position  = vec4(0);//g_mProj * g_mView * pos;
    Texcoord     = g_Texcoord;
}

//Fragment Shader
uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;
uniform vec3      g_vEyeDir;

varying vec2   Texcoord;
varying vec3   N;
varying vec3   V;
varying vec3   L;
varying float  Ldist;

varying vec3   normal;
varying vec3   ldir;
varying vec3   eye;

void main( void )
{
 /*vec4 color     = texture2D(g_txColor,    Texcoord);
 vec4 normal    = texture2D(g_txNormal,   Texcoord);
 vec4 specular  = texture2D(g_txSpecular, Texcoord);

 normal.xyz = normalize(normal.xyz + normalize(N));

 float intensity  = 1.0;
       intensity  = clamp(dot(normal.xyz, L), 0.1, 1.0);
//       intensity += dot(normal.xyz, g_vEyeDir);
       intensity  = clamp(intensity, 0.0, 1.0);

 vec3  col = (color.xyz + specular.xyz) * intensity;

 col = clamp(col, 0.0, 1.0);

 gl_FragColor = vec4(col.x, col.y, col.z, color.w);*/
    vec4 color     = texture2D(g_txColor,    Texcoord);
    vec4 specular  = texture2D(g_txSpecular, Texcoord);
    vec4 spec = vec4(0.0);

    vec3 n = normalize(normal);
    vec3 l = normalize(ldir);
    vec3 e = normalize(eye);

    float shininess = 1.0f;
    float intensity = max(dot(n,l), 0.0);

    if (intensity > 0.0) {
        vec3 h = normalize(l + e);
        float intSpec = max(dot(h,n), 0.0);
        spec = specular * pow(intSpec, shininess);
    }

    gl_FragColor = max(intensity * color + spec, color);
}
