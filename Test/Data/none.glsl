vec4 kgm_main()
{
  return ( g_mProj * g_mView * g_mTran * vec4(g_Vertex, 1.0) );
}

//Fragment Shader

vec4 main()
{
  return  g_vColor;
}
