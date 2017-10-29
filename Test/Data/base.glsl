varying vec4 PxColor;

void kgm_main(out vec4 pos)
{
   /*mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                     g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                     g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);*/

   v_V = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;

   v_UV = a_UV;

   PxColor = g_vColor;

   pos = ( g_mProj * g_mView * vec4(v_V, 1.0) );
}

//Fragment Shader
varying vec4   PxColor;

void kgm_main(out vec4 col)
{
  col = PxColor * texture2D(g_txColor, v_UV);
}
