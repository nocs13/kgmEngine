#version 120
uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;

attribute vec3 g_Vertex;

void main(void)
{
   gl_Position  = g_mProj * g_mView * g_mTran * g_Vertex;
}

//Fragment Shader

void main( void )
{
  gl_FragColor = vec4(1,1,1,1);
}
