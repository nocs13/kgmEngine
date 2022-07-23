#include "kgmGameShaders.h"

#define KGM_SHADER_MAX_LIGHT 16

const char* const begin_vshader =  "#version 120 \n"
"struct Light          \n"
"{                     \n"
"  vec4   pos;         \n"
"  vec4   dir;         \n"
"};                    \n"

"#define MAX_LIGHTS 16 \n"
"uniform mat4   g_mView;           \n"
"uniform mat4   g_mProj;           \n"
"uniform mat4   g_mTran;           \n"
"uniform vec4   g_vColor;          \n"
"uniform vec4   g_vSpecular;       \n"
"uniform vec4   g_vClipPlane;      \n"
"uniform vec3   g_vUp;             \n"
"uniform vec3   g_vEye;            \n"
"uniform vec3   g_vLook;           \n"
"uniform float  g_fTime;           \n"
"uniform float  g_fShine;          \n"
"uniform float  g_fRandom;         \n"
"uniform float  g_fAmbient;        \n"
"uniform float  g_fLightPower;     \n"
"uniform int    g_iClipping = 0;   \n"

"uniform Light  g_sLights[MAX_LIGHTS]; \n"

"varying vec3   v_N;               \n"
"varying vec3   v_V;               \n"
"varying vec3   v_L;               \n"
"varying vec3   v_L_color;         \n"
"varying float  v_L_power;         \n"
"varying vec3   v_Y;               \n"
"varying vec2   v_UV;              \n"
"varying float  v_I;               \n"
"varying float  v_shine;           \n"
"varying vec4   v_color;           \n"
"varying vec4   v_specular;        \n"

"attribute vec3 a_Vertex;          \n"
"attribute vec3 a_Normal;          \n"
"attribute vec4 a_Color;           \n"
"attribute vec2 a_UV;              \n";

const char* const end_vshader =   " \n"
"void main(void)                    \n"
"{                                  \n"
"   vec4 position;                  \n"
"   v_specular = g_vSpecular;       \n"
"   v_color = g_vColor * a_Color;   \n"
"   kgm_main(position);             \n"
"   gl_Position = position;         \n"
"}";

const char* const begin_pshader = "#version 120 \n"
"#define MAX_LIGHTS 8 \n"
"#ifdef GL_ES         \n"
"precision lowp float;\n"
"#endif               \n"

"uniform sampler2D g_txColor;\n"
"uniform sampler2D g_txNormal;\n"
"uniform sampler2D g_txSpecular;\n"

"varying vec3   v_N;\n"
"varying vec3   v_V;\n"
"varying vec3   v_L;\n"
"varying vec3   v_Y;\n"
"varying vec2   v_UV;\n"
"varying float  v_I;\n"
"varying float  v_shine;\n"
"varying vec4   v_color;\n"
"varying vec4   v_specular;\n";

const char* const end_pshader = "\n"
"void main( void )\n"
"{\n"
"    vec4 color;                     \n"
"    kgm_main(color);                \n"
"    gl_FragColor = color; \n"
"}";
