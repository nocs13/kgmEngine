#version 100

uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vColor;
uniform vec4   g_vClipPlane;

varying vec3   v_N;
varying vec3   v_V;

attribute vec3 a_Vertex;
attribute vec4 a_Color;

varying vec4  position;
varying vec4  color;

void process(out vec4 pos)
{
   position = g_mTran * vec4(a_Vertex, 1);
   pos = g_mProj * g_mView * position;
}

void main(void)
{
   vec4 position;
   color = a_Color;
   process(position);
   gl_Position = position;
}

//Fragment Shader
#version 100

#ifdef GL_ES
precision lowp float;
#endif

varying vec4  position;
varying vec4  color;

void main( void )
{
  gl_FragColor = color;
}
