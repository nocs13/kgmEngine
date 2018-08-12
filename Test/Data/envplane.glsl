varying vec4 PxColor;
varying vec4 clip;
varying vec4 position;
varying vec2 dudv;
varying float time;
varying float random;

const float tiling = 1.0;

void kgm_main(out vec4 pos)
{
   v_UV = a_UV;
   PxColor = g_vColor * a_Color;
   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
   clip = g_mProj * g_mView * g_mTran * vec4(a_Vertex.x, a_Vertex.y, 0, 1);
   dudv = vec2(a_Vertex.x / 2.0 + 0.5, a_Vertex.y / 2.0 + 0.5) * tiling;
   position = pos;
   time = g_fTime;
   random = g_fRandom;
}

//Fragment Shader
varying vec4 PxColor;
varying vec4 clip;
varying vec4 position;
varying vec2 dudv;
varying float time;
varying float random;

uniform sampler2D g_txEnvironment;

const float strength = 0.02;

void kgm_main(out vec4 col)
{
  vec2 distortion = (texture2D(g_txNormal, dudv).rg * 2.0 - 1.0) * strength;
  vec2 ndc = (clip.xy / clip.w) / 2.0 + 0.5;
  vec2 rlc = vec2(ndc.x, -ndc.y);

  col = v_color * PxColor * texture2D(g_txEnvironment, rlc + distortion);
  //col = vec4(1, 0, 0, 1);
}
