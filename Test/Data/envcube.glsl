varying vec4 PxColor;
varying vec3 txCoord;

void kgm_main(out vec4 pos)
{
   v_UV = a_UV;
   txCoord = a_Vertex;
   PxColor = g_vColor * a_Color;
   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec3 txCoord;
varying vec4 PxColor;
uniform samplerCube g_txEnvironment;

void kgm_main(out vec4 col)
{
  col = v_color * PxColor * textureCube(g_txEnvironment, -txCoord * 0.5);
  //col = vec4(1, 0, 0, 1);
}
