vec4 kgm_main(void)
{
   v_V = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;

   v_UV = a_UV;

   v_I = g_fAmbient;

   return (g_mProj * g_mView * vec4(v_V, 1.0));
}

//Fragment Shader

vec4 kgm_main( void )
{
  vec4 color = texture2D(g_txColor, v_UV);

  color *= g_vColor;

  color.xyz *= v_I;

  return color;
}
