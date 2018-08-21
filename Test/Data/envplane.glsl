uniform float  g_fForce;
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
varying float rrforce;

const float tiling = 1.0;

void kgm_main(out vec4 pos)
{
   vec4 wpos = g_mTran * vec4(a_Vertex, 1);

   v_UV = a_UV;
   PxColor = g_vColor * a_Color;
   rrforce = g_vColor.w;
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
uniform float  g_fForce   = 1.0;
uniform float  g_fFresnel = 1.0;
uniform float  g_fDistort = 0.0;
uniform vec3   g_vMove;

varying vec4 PxColor;
varying vec4 clip;
varying vec4 position;
varying vec3 look;
varying vec2 dudv;
varying float time;
varying float random;
varying float rrforce;

uniform sampler2D g_txEnvironment;

void kgm_main(out vec4 col)
{
  vec2 distortion = (texture2D(g_txNormal, vec2(dudv.x + g_vMove.x, dudv.y + g_vMove.y)).rg * 2.0 - 1.0) * g_fDistort;

  vec2 ndc = (clip.xy / clip.w) / 2.0 + 0.5;
  vec2 rrc = vec2(ndc.x,  ndc.y);
  vec2 rlc = vec2(ndc.x, -ndc.y);

  vec4 rlcol = texture2D(g_txEnvironment, rlc + distortion);
  vec4 rrcol = texture2D(g_txSpecular,    rrc - distortion);

  vec3 view = normalize(look);
  float mixfactor = (.5 - rrforce) * dot(look, vec3(0, 0, 1));

  col = mix(rlcol, rrcol, mixfactor);

  //col = texture2D(g_txEnvironment, rlc + distortion);
  //col = texture2D(g_txSpecular, rrc - distortion);
  col.w *= g_fForce;
}
