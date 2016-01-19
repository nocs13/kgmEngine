const char* const begin_vshader = "uniform mat4   g_mView;       \n"
"uniform mat4   g_mProj;       \n"
"uniform mat4   g_mTran;       \n"
"uniform vec4   g_vColor;      \n"
"uniform vec4   g_vLight;      \n"
"uniform vec3   g_vEye;        \n"
"uniform vec3   g_vEyeDir;     \n"
"uniform float  g_fTime;       \n"
"uniform float  g_fShine;      \n"
"uniform float  g_fRandom;     \n"
"uniform float  g_fAmbient;    \n"

"varying vec3   v_N;           \n"
"varying vec3   v_V;           \n"
"varying vec3   v_L;           \n"
"varying vec3   v_Y;           \n"
"varying vec2   v_UV;          \n"
"varying float  v_I;           \n"
"varying float  v_shine;       \n"
"varying vec4   v_color;       \n"

"attribute vec3 a_Vertex;      \n"
"attribute vec3 a_Normal;      \n"
"attribute vec4 a_Color;       \n"
"attribute vec2 a_UV;          \n";

const char* const end_vshader = "\n"
"void main(void)                   \n"
"{                                 \n"
"    vec4 position;                \n"
"    kgm_main(position);           \n"
"    v_color = g_vColor * a_Color; \n"
"    gl_Position = position;       \n"
"}";

const char* begin_pshader = "#ifdef GL_ES\n"
"precision lowp float;\n"
"#endif\n"

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
"varying vec4   v_color;\n";

const char* end_pshader = "\n"
"void main( void )\n"
"{\n"
"    vec4 color;                     \n"
"    kgm_main(color);                \n"
"    gl_FragColor = v_color * color; \n"
"}";