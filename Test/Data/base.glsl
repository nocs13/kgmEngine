vec4 kgm_main(void)
{
   mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                     g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                     g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

   v_V = vec4(g_mTran * vec4(g_Vertex, 1.0)).xyz;

   v_N = normalize(mRot * g_Normal);

   v_I = g_vLight.w;

   v_L = g_vLight.xyz;

   v_UV = a_UV;

   vec4 result = g_mProj * g_mView * vec4(V, 1.0);

   return result;
}

//Fragment Shader

vec4 kgm_main( void )
{
  vec4 color = g_vColor;

  return color;
}
