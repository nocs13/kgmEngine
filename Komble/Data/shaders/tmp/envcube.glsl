uniform float  g_fForce;
uniform float  g_fFresnel;
uniform vec3   g_vMove;

varying vec4 PxColor;
varying vec3 txCoord;
varying vec3 normal;
varying vec3 position;
varying vec2 dudv;

const float tiling = 1.0;

void kgm_main(out vec4 pos)
{
   vec3 normal = normalize(a_Normal);

   v_UV = a_UV;
   txCoord.x = a_Vertex.x;
   txCoord.y = -a_Vertex.z;
   txCoord.z = a_Vertex.y;
   PxColor = g_vColor * a_Color;
   dudv = vec2(a_Vertex.x / 2.0 + 0.5, a_Vertex.y / 2.0 + 0.5) * tiling;

   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec3 txCoord;
varying vec4 PxColor;
varying vec2 dudv;

uniform float  g_fForce   = 1.0;
uniform float  g_fFresnel = 1.0;
uniform float  g_fDistort = 0.0;
uniform vec3   g_vMove;

uniform samplerCube g_txEnvironment;

void kgm_main(out vec4 col)
{
  vec3 distortion = (texture2D(g_txNormal, vec2(dudv.x + g_vMove.x, dudv.y + g_vMove.y)).rgb * 2.0 - 1.0) * g_fDistort;

  vec4 rfcol = PxColor * textureCube(g_txEnvironment, txCoord + distortion);

  col = rfcol;
}
