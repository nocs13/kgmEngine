/*varying vec4   PxColor;

void kgm_main(out vec4 pos)
{
   v_UV     = a_UV;
   PxColor  = a_Color;

   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}*/

//Fragment Shader
/*varying vec4   PxColor;

void kgm_main(out vec4 col)
{
 vec4 color  = texture2D(g_txColor, v_UV);

 col = color * PxColor;
}*/
