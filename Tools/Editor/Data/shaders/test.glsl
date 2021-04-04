void kgm_main(out vec4 pos)
{
  pos = g_mProj * g_mView * vec4(a_Vertex, 1);
}
//Fragment Shader
void kgm_main(out vec4 col)
{
  col = v_color;
}
