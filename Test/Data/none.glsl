#version 120
uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;

attribute vec3 g_Vertex;

void main(void)
{
   gl_Position  = g_mProj * g_mView * g_mTran * vec4(g_Vertex, 1);
}

//Fragment Shader

void main( void )
{
  gl_FragColor = vec4(1,0,0,1);
}
