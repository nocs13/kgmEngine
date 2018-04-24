uniform mat4 g_mLight;

varying vec4 q;

void kgm_main(out vec4 pos)
{
  q = g_mLight * g_mTran * vec4(a_Vertex, 1);

  pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
uniform sampler2DShadow txDepth;
//uniform sampler2D txDepth;

varying vec4 q;

void kgm_main(out vec4 col)
{
  //vec3 scoord = 0.5 * (q.xyz / q.w) + 0.5;
  vec3 scoord = q.xyz / q.w;
  scoord.xy = 0.5 * scoord.xy + 0.5;

  /*scoord.xy = clamp(scoord.xy, 0.01, 0.99);

  if((scoord.x < 0.0) || (scoord.x > 1.0))
    discard;

  if((scoord.y < 0.0) || (scoord.y > 1.0))
    discard;

  scoord = clamp(scoord, 0.0, 1.0);*/

  //float shadow = 1.0;
  float shadow = shadow2D(txDepth, scoord).r;
  //float shadow = texture2D(txDepth, scoord.xy).r;
  float depth  = scoord.z;

  //vec3 color = vec3(shadow, 0.0, depth);
  vec3 color = vec3(shadow, shadow, shadow);

  //if (shadow < 0.0000000001)
  //  discard;

  if ((shadow - depth) > 0.000001)
    discard;

  if(shadow > 0.9)
    discard;

  //color *= shadow;
  col = vec4(color.xyz, 0.5);
}
