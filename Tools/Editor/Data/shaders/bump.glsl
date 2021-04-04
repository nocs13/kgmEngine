uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vAmbient;
uniform vec4   g_vLight;
uniform vec3   g_vEye;
uniform vec3   g_vEyeDir;
uniform float  g_fTime;
uniform float  g_fShine;
uniform float  g_fAlpha;
uniform float  g_fRandom;

varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec3   Y;
varying vec2   Texcoord;
varying float  I;
varying float  shine;
varying float  alpha;

attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

void main(void)
{
    mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
            g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
            g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

    V = vec4(g_mTran * vec4(g_Vertex, 1.0)).xyz;

    N = normalize(mRot * g_Normal);
    //N = normalize(g_Normal);

    I = g_vLight.w;
    L = g_vLight.xyz;
    Y = g_vEyeDir;

    shine = g_fShine;
    alpha = g_fAlpha;

    gl_Position   = g_mProj * g_mView * vec4(V, 1.0);
    Texcoord      = g_Texcoord;
}

//Fragment Shader
#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;

varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec3   Y;
varying vec2   Texcoord;
varying float  I;
varying float  shine;
varying float  alpha;

void main( void )
{
    vec4 bump = texture2D(g_txNormal, Texcoord);
    vec4 spec = texture2D(g_txSpecular, Texcoord);

    vec3 LN = normalize(L - V);

    vec3 norm = normalize(N + bump.xyz);
    //norm = normalize(bump.xyz);

    float distance = length(L - V);
    float intensity = I * dot(norm, LN) / (1.0 + distance);

    vec4  col = vec4(intensity, intensity, intensity, 1.0);

    //if(dot(N, LN) > 0.0)
    {
      vec4 specular;
      specular = spec * pow(max(0.0, dot(reflect(LN, norm), -Y)), shine);
      col.xyz += specular.xyz;
    }

    col.xyz = clamp(col.xyz, 0.0, 1.0);

    gl_FragColor = col;
}
