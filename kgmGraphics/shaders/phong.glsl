#version 100

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
uniform vec4   g_vClipPlane;
uniform vec3   g_vUp;
uniform vec3   g_vEye;
uniform vec3   g_vLook;
uniform float  g_fTime;
uniform float  g_fShine;
uniform float  g_fRandom;
uniform float  g_fAmbient;
uniform float  g_fLightPower;
uniform int    g_iClipping;
uniform int    g_iLights;

uniform Light  g_sLights[MAX_LIGHTS];

varying vec3   v_P;
varying vec3   v_V;
varying vec3   v_N;
varying vec3   v_Y;
varying vec2   v_UV;
varying float  v_I;
varying float  v_shine;
varying vec4   v_color;

attribute vec3 a_Vertex;
attribute vec3 a_Normal;
attribute vec4 a_Color;
attribute vec2 a_UV;

float det(mat2 m) {
    return m[0].x * m[1].y - m[0].y * m[1].x;
}

mat3 inverse(mat3 m) {
    vec3 r0 = m[0];
    vec3 r1 = m[1];
    vec3 r2 = m[2];

    vec3 m0 = vec3(
        det(mat2(r1.y, r1.z, r2.y, r2.z)),
        det(mat2(r1.z, r1.x, r2.z, r2.x)),
        det(mat2(r1.x, r1.y, r2.x, r2.y))
    );
    vec3 m1 = vec3(
        det(mat2(r2.y, r2.z, r0.y, r0.z)),
        det(mat2(r2.z, r2.x, r0.z, r0.x)),
        det(mat2(r2.x, r2.y, r0.x, r0.y))
    );
    vec3 m2 = vec3(
        det(mat2(r0.y, r0.z, r1.y, r1.z)),
        det(mat2(r0.z, r0.x, r1.z, r1.x)),
        det(mat2(r0.x, r0.y, r1.x, r1.y))
    );

    mat3 adj = mat3(0.0); //transpose(mat3(m0, m1, m2));

    return (1.0 / dot(r0, m0)) * adj;
}

void process(out vec4 pos)
{
  v_UV = a_UV;
  v_Y  = g_vEye;
  v_P  = vec3(g_mTran * vec4(a_Vertex, 1));
  mat3 m_N = mat3(g_mTran);
  m_N = inverse(m_N);
  //m_N = transpose(m_N);
  v_N  = normalize( m_N * a_Normal );
  //v_N  = normalize(a_Normal);
  v_V  = vec3(g_mView * vec4(v_P, 1));
  pos  = g_mProj * vec4(v_V, 1);
  v_color    = g_vColor;// * a_Color;
  v_shine    = g_fShine;
}

void main(void)
{
  vec4 pos;
  process(pos);
  gl_Position = pos;
}

//Fragment Shader
#version 100

#ifdef GL_ES
//precision lowp float;
precision highp float;
#endif

struct Light
{
  vec4   pos;
  vec4   dir;
  vec4   col;
};

#define MAX_LIGHTS 16
#define M_PI 3.14159265359

uniform int    g_iLights;
uniform Light  g_sLights[MAX_LIGHTS];

uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;

varying vec3   v_P;
varying vec3   v_V;
varying vec3   v_N;
varying vec3   v_Y;
varying vec2   v_UV;
varying float  v_I;
varying float  v_shine;
varying vec4   v_color;

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
  vec3 cbase = vec3( v_color * texture2D(g_txColor, v_UV) );
  vec3 sbase = vec3( v_color * texture2D(g_txSpecular, v_UV) );
  vec3 shade = vec3(0.0, 0.0, 0.0);
  vec3 shine = vec3(0.0, 0.0, 0.0);

  vec3 vnor = normalize(v_N);

  for (int i = 0; i < g_iLights; i++)
  {
    vec3 lpos = g_sLights[i].pos.xyz;
    vec3 ldir = g_sLights[i].dir.xyz;
    vec3 lcol = g_sLights[i].col.rgb;

    float power = g_sLights[i].pos.w;
    float angle = g_sLights[i].dir.w;
    float dsize = abs( length(lpos - v_P) );


    //if (angle > M_PI)
    {
      //dsize = 1.0;
      ldir = normalize(v_P - lpos);
    }

    float lambert = dot(-ldir, vnor) * (power / dsize);

    lambert = max(lambert, 0.0);

    shade.rgb += (lcol * lambert);

    if (lambert > 0.0 && v_shine > 0.0)
    {
      vec3 R = reflect(ldir, vnor);            // Reflected light vector
      vec3 V = normalize(v_P - v_Y);           // Vector to viewer
      // Compute the specular term
      float angle = max(dot(R, -V), 0.0);
      float specular = pow(angle, v_shine);
      shine += (specular / dsize);
    }
  }

  //shine = normalize(shine);

  col.rgb = cbase * shade + shine * sbase;
  //col.rgb = shine;
  //col.rgb = shade;
  col.w = 1.0;
}

void main( void )
{
    vec4 col;
    process(col);
    //col = vec4(1, 0, 0, 1);
    gl_FragColor = col;
}
