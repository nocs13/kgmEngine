varying vec4  position;
varying float clipping;

void kgm_main(out vec4 pos)
{
   v_UV = a_UV;
   position = g_mTran * vec4(a_Vertex, 1);
   pos = g_mProj * g_mView * position;

   if (g_iClipping > 0)
   {
     clipping = dot(position, g_vClipPlane);
   }
   else
   {
     clipping = 1.0;
   }
}

//Fragment Shader
varying vec4  position;
varying float clipping;

void kgm_main(out vec4 col)
{
  if (clipping < 0.0)
    discard;

  col = v_color * texture2D(g_txColor, v_UV);
}
