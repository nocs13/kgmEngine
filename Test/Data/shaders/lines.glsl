varying vec4 color;

void kgm_main(out vec4 pos)
{
  pos = ( g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1.0) );
  
  color = a_Color;
}

//Fragment Shader
varying vec4 color;

void kgm_main(out vec4 col)
{
  col = color;
}
