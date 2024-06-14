uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;

attribute vec3 g_Vertex;
attribute vec4 g_Color;

varying   vec4 vColor;

void main()
{
   gl_Position  = g_mProj * g_mView * g_mTran * vec4(g_Vertex, 1.0);
   vColor = g_Color;
}

//Fragment Shader
#ifdef GL_ES
precision highp float;
#endif

varying vec4 vColor;

void main()
{
  gl_FragColor = vColor;
}
