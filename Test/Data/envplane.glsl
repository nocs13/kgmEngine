uniform float  g_fFresnel;
uniform vec3   g_vMove;

varying vec4 PxColor;
varying vec4 clip;
varying vec4 position;
varying vec3 eye;
varying vec3 look;
varying vec2 dudv;
varying float time;
varying float random;

const float tiling = 1.0;

void kgm_main(out vec4 pos)
{
   vec4 wpos = g_mTran * vec4(a_Vertex, 1);

   v_UV = a_UV;
   PxColor = g_vColor * a_Color;
   pos = g_mProj * g_mView * wpos;
   clip = g_mProj * g_mView * g_mTran * vec4(a_Vertex.x, a_Vertex.y, 0, 1);
   dudv = vec2(a_Vertex.x / 2.0 + 0.5, a_Vertex.y / 2.0 + 0.5) * tiling;
   position = pos;
   time = g_fTime;
   random = g_fRandom;
   eye = g_vEye;
   look = eye - wpos.xyz;
}

//Fragment Shader
uniform float  g_fFresnel;
uniform vec3   g_vMove;

varying vec4 PxColor;
varying vec4 clip;
varying vec4 position;
varying vec3 look;
varying vec2 dudv;
varying float time;
varying float random;

uniform sampler2D g_txEnvironment;

const float strength = 0.3;

void kgm_main(out vec4 col)
{
  vec2 distortion = (texture2D(g_txNormal, vec2(dudv.x + g_vMove.x, dudv.y)).rg * 2.0 - 1.0) * strength;
  vec2 ndc = (clip.xy / clip.w) / 2.0 + 0.5;
  vec2 rlc = vec2(ndc.x, -ndc.y);

  col = v_color * PxColor * texture2D(g_txEnvironment, rlc + distortion);

  vec3 view = normalize(look);
  float rffactor = dot(view, vec3(0, 1, 0));
  rffactor = pow(rffactor, 1.0);

  col = mix(col, v_color, rffactor);
}
