varying vec4   C;
varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec3   Y;
varying vec3   VV;
varying float  I;
varying float  shine;
varying float  alpha;

vec4 kgm_main(void)
{
    mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                      g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                      g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

    V  = vec4(g_mTran * vec4(g_Vertex, 1.0)).xyz;
    VV = vec4(g_mView * vec4(V, 1.0)).xyz;

    N = normalize(mRot * a_Normal);

    I = g_vLight.w;
    L = g_vLight.xyz;
    //Y = g_vEyeDir;
    Y = -vec3(g_mView * vec4(V, 1.0));
    C = g_Color;

    shine = g_fShine;
    alpha = g_vColor.w;

    Texcoord      = a_Texcoord;

    return ( g_mProj * g_mView * vec4(V, 1.0) );
}

//Fragment Shader

varying vec4   C;
varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec3   Y;
varying vec3   VV;
varying vec2   Texcoord;
varying float  I;
varying float  shine;
varying float  alpha;

vec4 kgm_main( void )
{

    vec3 NN = normalize(N);
    vec3 LN = normalize(L - V);
    vec3 R  = normalize(-reflect(LN, NN));
    vec3 E  = normalize(Y - VV); //VV

    //Normal
    {
      vec3 bump = texture2D(g_txNormal, Texcoord).xyz * 2.0 - 1.0;
      NN = normalize(NN + bump);
    }

    float distance = 1.0 + length(L - V);
    float intensity = max(0.0, (I * dot(NN, LN) / distance));

    vec4 col = C * texture2D(g_txColor, Texcoord);
    col.xyz *= intensity;

    //Specular
    {
      vec3 specular = texture2D(g_txSpecular, Texcoord).rgb;
      specular += vec3(intensity * pow(max(0.0, dot(R, E)), shine));
      specular = clamp(specular, 0.0, 1.0);
      col.xyz = col.xyz + specular;
    }

    col.xyz = clamp(col.xyz, 0.0, 1.0);
    return vec4(col.xyz, alpha * col.w);
}
