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


  //float shadow = 1.0;
  float shadow = shadow2D(txDepth, scoord).r;
  //float shadow = texture2D(txDepth, scoord.xy).r;
  float depth  = scoord.z;

  //if ((shadow + 0.0001) >= depth)
  //    discard;

  float d = depth - shadow;
  //d = clamp(d, 0.0, 1.0);
  //vec3 color = vec3(depth, depth, depth);
  //vec3 color = vec3(shadow, shadow, shadow);
  vec3 color = vec3(d, 0, 0);

  //if (d < 0.000001)
  //  discard;

  //color *= shadow;
  col = vec4(color.xyz, 1.0);
}
