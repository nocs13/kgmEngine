void kgm_main(out vec4 pos)
{
  pos = ( g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1.0) );
}

//Fragment Shader

void kgm_main(out vec4 col)
{
  col = g_vColor;
}
