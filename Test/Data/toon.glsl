varying vec3 L_Dir;

void kgm_main(out vec4 pos)
{
  mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                    g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                    g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

  v_V  = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;

  //VV = vec4(g_mView * vec4(v_V, 1.0)).xyz;

  v_N = normalize(mRot * a_Normal);

  if (length(g_vLightDirection.xyz) == 0.0)
  {
    L_Dir = g_vLight.xyz - v_V.xyz;
  }
  else
  {
    L_Dir = g_vLightDirection.xyz;
  }

  v_UV = a_UV;

  pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec3 L_Dir;

void kgm_main(out vec4 col)
{
  float intensity;
  vec4 color;

  intensity = dot(normalize(L_Dir), normalize(v_N));

  if (intensity > 0.95)
    color = vec4(1.0, 1.0, 1.0, 1.0);
  else if (intensity > 0.5)
    color = vec4(0.6, 0.6, 0.6, 1.0);
  else if (intensity > 0.25)
    color = vec4(0.4, 0.4, 0.4, 1.0);
  else
    color = vec4(0.2, 0.2, 0.2, 1.0);

  //col = color * texture2D(g_txColor, v_UV);
  col = color;
}
