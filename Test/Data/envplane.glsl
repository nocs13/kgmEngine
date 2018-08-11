varying vec4 PxColor;
varying vec4 clip;

void kgm_main(out vec4 pos)
{
   v_UV = a_UV;
   PxColor = g_vColor * a_Color;
   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
   clip = g_mProj * g_mView * g_mTran * vec4(a_Vertex.x, a_Vertex.y, 0, 1);
}

//Fragment Shader
varying vec4 PxColor;
varying vec4 clip;
uniform sampler2D g_txEnvironment;

void kgm_main(out vec4 col)
{
  vec2 ndc = (clip.xy / clip.w) / 2.0 + 0.5;
  vec2 rlc = vec2(ndc.x, -ndc.y);
  col = v_color * PxColor * texture2D(g_txEnvironment, rlc);
  //col = vec4(1, 0, 0, 1);
}
