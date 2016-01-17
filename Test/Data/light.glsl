varying vec3   VV;
varying vec3   eye;

void kgm_main(out vec4 pos)
{
    mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                      g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                      g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

    v_V  = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;
    VV = vec4(g_mView * vec4(v_V, 1.0)).xyz;

    v_N = normalize(mRot * a_Normal);

    v_I = g_vLight.w;
    v_L = g_vLight.xyz;
    eye = g_vEye;
    v_Y = -vec3(g_mView * vec4(v_V, 1.0));

    v_shine = g_fShine;

    v_UV = a_UV;

    pos = ( g_mProj * g_mView * vec4(v_V, 1.0) );
}

//Fragment Shader

varying vec3   VV;
varying vec3   eye;

void kgm_main( out vec4 color )
{

    vec3 NN = normalize(v_N);
    vec3 LN = normalize(v_L - v_V);
    vec3 R  = normalize(-reflect(LN, NN));
    vec3 E  = normalize(v_Y - VV); //VV

    //Normal
    {
      vec3 bump = texture2D(g_txNormal, v_UV).xyz * 2.0 - 1.0;
      bump = normalize(bump);
      NN = normalize(NN + bump);
    }

    float distance = 1.0 + length(v_L - v_V);
    float intensity = max(0.0, (v_I * dot(NN, LN) / distance));

    vec4 col = texture2D(g_txColor, v_UV);
    col.xyz *= intensity;

    //Specular
    if(v_shine > 1.0)
    {
      vec3 specular = texture2D(g_txSpecular, v_UV).rgb;
      vec3 viewDir = normalize(eye - v_V);
      vec3 reflDir = reflect(-LN, NN);
      float spec = pow(max(dot(viewDir, reflDir), 0.0), v_shine);
      //specular += vec3(intensity * pow(max(0.0, dot(R, E)), v_shine));
      //specular += vec3(intensity * pow(max(0.0, dot(R, E)), v_shine));
      //specular = clamp(specular, 0.0, 1.0);
      specular = specular * spec * 0.5 + spec * 0.5;
      //col.xyz = col.xyz * specular;
      col.xyz += specular;
    }

    col.xyz = clamp(col.xyz, 0.0, 1.0);

    color = col;
}
