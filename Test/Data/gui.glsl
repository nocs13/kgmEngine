varying vec4 PxColor;

void kgm_main(out vec4 pos)
{
   v_UV = a_UV;
   PxColor = g_vColor;
   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec4 PxColor;

void kgm_main(out vec4 col)
{
  col = PxColor * texture2D(g_txColor, v_UV);
}
