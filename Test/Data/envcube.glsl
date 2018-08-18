uniform float  g_fForce;
uniform float  g_fFresnel;
uniform vec3   g_vMove;

varying vec4 clip;
varying vec4 PxColor;
varying vec3 txCoord;
varying vec3 normal;
varying vec3 position;
varying vec2 dudv;

varying vec4 projTexCoord;

const float tiling = 1.0;

void kgm_main(out vec4 pos)
{
   vec3 normal = normalize(a_Normal);

   v_UV = a_UV;
   txCoord.x = a_Vertex.x;
   txCoord.y = -a_Vertex.z;
   txCoord.z = a_Vertex.y;
   PxColor = g_vColor * a_Color;
   //txCoord = (transpose(g_mView * g_mTran) * vec4(normal, 1.0)).xyz;
   //txCoord = (transpose(g_mView * g_mTran) * vec4(a_Vertex, 1.0)).xyz;
   //position = g_mView * g_mTran * vec4(a_Vertex, 0.0)
   clip = g_mProj * g_mView * g_mTran * vec4(a_Vertex.x, a_Vertex.y, a_Vertex.z, 1);
   dudv = vec2(a_Vertex.x / 2.0 + 0.5, a_Vertex.y / 2.0 + 0.5) * tiling;
   projTextCoord = g_mProj *  g_mTran * vec4(a_Vertex, 1);

   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec3 txCoord;
varying vec4 PxColor;
varying vec4 clip;
varying vec2 dudv;

varying vec4 projTexCoord;

uniform float  g_fForce   = 1.0;
uniform float  g_fFresnel = 1.0;
uniform float  g_fDistort = 0.0;
uniform vec3   g_vMove;

uniform samplerCube g_txEnvironment;

void kgm_main(out vec4 col)
{
  vec2 distortion = (texture2D(g_txNormal, vec2(dudv.x + g_vMove.x, dudv.y + g_vMove.y)).rg * 2.0 - 1.0) * g_fDistort;

  vec2 ndc = (clip.xy / clip.w) / 2.0 + 0.5;
  vec2 rrc = vec2(ndc.x,  ndc.y);
  //vec4 rrcol = texture2D(g_txSpecular, ndc);//rrc - distortion);
  vec4 rrcol = textureProj(g_txSpecular, projTexCoord);//rrc - distortion);
  vec4 rfcol = PxColor * textureCube(g_txEnvironment, txCoord);

  //col = mix(rfcol, rrcol, 0.5);
  col = rrcol;
  //col = vec4(1, 0, 0, 1);
}
