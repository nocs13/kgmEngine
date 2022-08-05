#version 120

struct Light
{
  vec4   pos;
  vec4   dir;
  vec4   col;
};

#define MAX_LIGHTS 16

uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vColor;
uniform vec4   g_vSpecular;
uniform vec4   g_vClipPlane;
uniform vec3   g_vUp;
uniform vec3   g_vEye;
uniform vec3   g_vLook;
uniform float  g_fTime;
uniform float  g_fShine;
uniform float  g_fRandom;
uniform float  g_fAmbient;
uniform float  g_fLightPower;
uniform int    g_iClipping = 0;
uniform int    g_iLights;

uniform Light  g_sLights[MAX_LIGHTS];

varying vec3   v_P;
varying vec3   v_N;
varying vec3   v_V;
varying vec3   v_Y;
varying vec2   v_UV;
varying float  v_I;
varying float  v_shine;
varying vec4   v_color;
varying vec4   v_specular;

attribute vec3 a_Vertex;
attribute vec3 a_Normal;
attribute vec4 a_Color;
attribute vec2 a_UV;              ;

void process(out vec4 pos)
{
  v_UV = a_UV;
  v_P  = vec3(g_mTran * vec4(a_Vertex, 1));
  //v_N  = vec3(g_mTran * vec4(a_Normal, 1));
  v_N = a_Normal;
  pos = g_mProj * g_mView * vec4(v_P, 1);
  v_color    = g_vColor;// * a_Color;
  v_specular = g_vSpecular;
}

void main(void)
{
  vec4 pos;
  process(pos);
  gl_Position = pos;
}

//Fragment Shader
#version 120

struct Light
{
  vec4   pos;
  vec4   dir;
  vec4   col;
};

#ifdef GL_ES
precision lowp float;
#endif

#define MAX_LIGHTS 16
#define M_PI 3.14159265359

uniform int    g_iLights;
uniform Light  g_sLights[MAX_LIGHTS];

uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;

varying vec3   v_P;
varying vec3   v_N;
varying vec3   v_V;
varying vec3   v_Y;
varying vec2   v_UV;
varying float  v_I;
varying float  v_shine;
varying vec4   v_color;
varying vec4   v_specular;

varying vec4  position;
varying float clipping;

/*
void col256torgb(in int c256, out vec3 crgb)
{
  crgb.r = (c256 >> 5) * 255 / 7;
  crgb.g = ((c256 >> 2) & 0x07) * 255 / 7;
  crgb.b = (c256 & 0x03) * 255 / 3;
}
8bit Color = (Red * 7 / 255) << 5 + (Green * 7 / 255) << 2 + (Blue * 3 / 255)
int  ilcol = 1000 * int(g_sLights[i].pos.w - power);
*/

void process(out vec4 col)
{
  vec3 col_base = vec3( v_color * texture2D(g_txColor, v_UV) );

  //for (int i = 0; i < g_iLights; i++)
  for (int i = 0; i < 1; i++)
  {
    vec3 lpos = vec3(1.0, 1.0, 10);//g_sLights[i].pos);
    vec3 ldir = vec3(0, 0, 0); //g_sLights[i].dir);

    float power = 1.0;//g_sLights[i].pos.w;
    float angle = g_sLights[i].dir.w;

    vec3 lcol  = vec3(g_sLights[i].col);

    //if (angle > M_PI)
    {
      ldir = normalize(lpos - v_P);
    }

    float r1 = normalize(dot(ldir, v_N));

    r1 = max(r1, 0.0);

    col.rgb += (col_base * r1 );
  }

  col.w = 1.0;
}

void main( void )
{
    vec4 col;
    process(col);
    //col = vec4(1, 0, 0, 1);
    gl_FragColor = col;
}
