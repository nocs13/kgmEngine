varying vec4   PxColor;

vec4 kgm_main(void)
{
   v_UV     = a_UV;
   PxColor  = a_Color;

   return g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec4   PxColor;

vec4 kgm_main( void )
{
 vec4 color  = texture2D(g_txColor, v_UV);

 return color * PxColor;
}
