varying vec4 PxColor;

void kgm_main(out vec4 pos)
{
   v_UV = a_UV;
   PxColor = g_vColor * a_Color;
   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec4   PxColor;
uniform samplerCube g_txEnvironment;

void kgm_main(out vec4 col)
{
  col = v_color * PxColor;// * texture(g_txEnvironment, v_UV);
}
