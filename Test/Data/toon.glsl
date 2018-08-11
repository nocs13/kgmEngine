varying vec3 VV;
varying vec3 L_Dir;
varying vec3 Y_Dir;
varying vec3   eye;

void kgm_main(out vec4 pos)
{
  mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                    g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                    g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

  v_V  = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;

  v_N = normalize(mRot * a_Normal);

  VV = vec4(g_mView * vec4(v_V, 1.0)).xyz;

  v_I = g_vLight.w;

  v_L = g_vLight.xyz;

  eye = g_vEye;

  v_Y = -vec3(g_mView * vec4(v_V, 1.0));

  Y_Dir = g_vEyeDir;

  if (length(g_vLightDirection.xyz) == 0.0)
  {
    L_Dir = g_vLight.xyz - v_V.xyz;
  }
  else
  {
    L_Dir = g_vLightDirection.xyz;
  }

  v_UV = a_UV;

  v_shine = g_fShine;


  pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec3  VV;
varying vec3  L_Dir;
varying vec3  Y_Dir;
varying vec3  eye;
varying vec3  specular;

void kgm_main(out vec4 col)
{
  vec3 NN = normalize(v_N);
  vec3 LN = normalize(v_L - v_V);
  vec3 R  = normalize(-reflect(LN, NN));
  vec3 E  = normalize(v_Y - VV);

  float intensity;
  vec4 color =  texture2D(g_txColor, v_UV);
       color.rgb *= (v_color.rgb * v_L_color);
       color.a   *= v_color.a;

  intensity = dot(normalize(L_Dir), normalize(v_N));

  if (intensity > 0.95)
    intensity = 1.0;
  else if (intensity > 0.5)
    intensity = 0.5;
  else if (intensity > 0.3)
    intensity = 0.3;
  else
    intensity = 0.2;

  //Specular
  if(v_shine > 1.5)
  {
    vec3  specular = texture2D(g_txSpecular, v_UV).rgb;
    vec3  viewDir  = normalize(eye - v_V);
    vec3  reflDir  = reflect(-LN, NN);
    float spec     = pow(max(dot(viewDir, reflDir), 0.0), v_shine);
    specular = specular * spec;

    color.xyz += specular;
  }

  color.xyz *= intensity;
  color.xyz = clamp(color.xyz, 0.0, 1.0);

  col = color;
}
