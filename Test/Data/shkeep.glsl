void kgm_main(out vec4 pos)
{
  pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader

void kgm_main(out vec4 col)
{
  //col = gl_FragCoord.z;
  gl_FragDepth = gl_FragCoord.z;
}
