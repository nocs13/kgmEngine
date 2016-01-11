const char* begin_vshader = ""
"uniform mat4   g_mView;"
"uniform mat4   g_mProj;"
"uniform mat4   g_mTran;"
"uniform vec4   g_vAmbient;"
"uniform vec4   g_vLight;"
"uniform vec3   g_vEye;"
"uniform vec3   g_vEyeDir;"
"uniform float  g_fTime;"
"uniform float  g_fShine;"
"uniform float  g_fAlpha;"
"uniform float  g_fRandom;"

"varying vec3   v_N;"
"varying vec3   v_V;"
"varying vec3   v_L;"
"varying vec3   v_Y;"
"varying vec2   v_Texcoord;"
"varying float  v_I;"
"varying float  v_shine;"
"varying float  v_alpha;"

"attribute vec3 a_Vertex;"
"attribute vec3 a_Normal;"
"attribute vec4 a_Color;"
"attribute vec2 a_Texcoord;";

const char* end_vshader = ""
"void main(void)"
"{"
"    gl_Position = kgm_main();"
"}";

const char* begin_pshader = ""
"#ifdef GL_ES"
"precision lowp float;"
"#endif"

"uniform sampler2D g_txColor;"
"uniform sampler2D g_txNormal;"
"uniform sampler2D g_txSpecular;"

"varying vec3   v_N;"
"varying vec3   v_V;"
"varying vec3   v_L;"
"varying vec3   v_Y;"
"varying vec2   v_Texcoord;"
"varying float  v_I;"
"varying float  v_shine;"
"varying float  v_alpha;";

const char* end_pshader = ""
"void main( void )"
"{"
"    vec4 color = kgm_main();"
"    gl_FragColor = color;"
"}";

