#include "kgmGLES.h"

#define GLES_2

kgmIGC* kgmCreateGLESContext(kgmWindow* w)
{
  #ifdef GLES_2
  return new kgmGLES(w);
  #else
  return null;
  #endif
}

#ifdef GLES_2

// kgmGLES.cpp: implementation of the kgmGLES class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmWindow.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmArray.h"

#define GL_DEPTH_STENCIL_EXT               GL_DEPTH_STENCIL_NV
#define GL_DEPTH_STENCIL_ATTACHMENT_EXT    0x821A
#define GLX_CONTEXT_PROFILE_MASK_ARB       0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB   0x00000001

struct {
  PFNEGLGETPROCADDRESSPROC eglGetProcAddress = null;
  PFNEGLBINDAPIPROC        eglBindAPI = null;
  PFNEGLQUERYSURFACEPROC   eglQuerySurface = null;
  PFNEGLGETDISPLAYPROC     eglGetDisplay = null;
  PFNEGLINITIALIZEPROC     eglInitialize = null;
  PFNEGLCHOOSECONFIGPROC   eglChooseConfig = null;
  PFNEGLCREATEWINDOWSURFACEPROC eglCreateWindowSurface = null;
  PFNEGLCREATECONTEXTPROC   eglCreateContext = null;
  PFNEGLMAKECURRENTPROC     eglMakeCurrent = null;
  PFNEGLQUERYCONTEXTPROC    eglQueryContext = null;
  PFNEGLSWAPINTERVALPROC    eglSwapInterval = null;
  PFNEGLDESTROYCONTEXTPROC  eglDestroyContext = null;
  PFNEGLDESTROYSURFACEPROC  eglDestroySurface = null;
  PFNEGLTERMINATEPROC       eglTerminate = null;
  PFNEGLSWAPBUFFERSPROC     eglSwapBuffers = null;
  PFNEGLGETCURRENTCONTEXTPROC eglGetCurrentContext = null;
  PFNEGLQUERYSTRINGPROC     eglQueryString = null;
  PFNEGLGETERRORPROC        eglGetError = null;

  PFNGLDEBUGMESSAGECALLBACKKHRPROC eglDebugMessageCallbackKHR = null;
} egl;

struct {
  PFNGLGETSTRINGPROC   glGetString = null;
  PFNGLENABLEPROC      glEnable = null;
  PFNGLDISABLEPROC     glDisable = null;
  PFNGLGETERRORPROC    glGetError = null;
  PFNGLGETINTEGERVPROC glGetIntegerv = null;
  PFNGLDEPTHFUNCPROC   glDepthFunc = null;
  PFNGLCULLFACEPROC    glCullFace = null;
  PFNGLCOLORMASKPROC   glColorMask = null;
  PFNGLCLEARDEPTHFPROC glClearDepthf = null;
  PFNGLCLEARSTENCILPROC glClearStencil = null;
  PFNGLCLEARCOLORPROC  glClearColor = null;
  PFNGLCLEARPROC       glClear = null;
  PFNGLFLUSHPROC       glFlush = null;
  PFNGLBLENDEQUATIONPROC glBlendEquation = null;
  PFNGLVIEWPORTPROC    glViewport = null;
  PFNGLBLENDFUNCPROC   glBlendFunc = null;
  PFNGLDEPTHMASKPROC   glDepthMask = null;
  PFNGLBINDTEXTUREPROC glBindTexture = null;
  PFNGLGENTEXTURESPROC glGenTextures = null;
  PFNGLTEXPARAMETERIPROC glTexParameteri = null;
  PFNGLTEXSUBIMAGE2DPROC glTexImage2D = null;
  PFNGLDELETETEXTURESPROC glDeleteTextures = null;
  PFNGLACTIVETEXTUREPROC glActiveTexture = null;
  PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = null;
  PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = null;
  PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = null;
  PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = null;
  PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = null;
  PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = null;
  PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = null;
  PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = null;
  PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = null;
  PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = null;
  PFNGLSCISSORPROC glScissor = null;
  PFNGLBINDBUFFERPROC glBindBuffer = null;
  PFNGLBUFFERDATAPROC glBufferData = null;
  PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = null;
  PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = null;
  PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = null;
  PFNGLDRAWELEMENTSPROC glDrawElements = null;
  PFNGLDRAWARRAYSPROC glDrawArrays = null;
  PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = null;
  PFNGLGENBUFFERSPROC glGenBuffers = null;
  PFNGLDELETEBUFFERSPROC glDeleteBuffers = null;
  PFNGLCREATESHADERPROC glCreateShader = null;
  PFNGLSHADERSOURCEPROC glShaderSource = null;
  PFNGLCOMPILESHADERPROC glCompileShader = null;
  PFNGLGETSHADERIVPROC glGetShaderiv = null;
  PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = null;
  PFNGLDELETESHADERPROC glDeleteShader = null;
  PFNGLATTACHSHADERPROC glAttachShader = null;
  PFNGLLINKPROGRAMPROC glLinkProgram = null;
  PFNGLGETPROGRAMIVPROC glGetProgramiv = null;
  PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = null;
  PFNGLCREATEPROGRAMPROC glCreateProgram = null;
  PFNGLDELETEPROGRAMPROC glDeleteProgram = null;
  PFNGLVALIDATEPROGRAMPROC glValidateProgram = null;
  PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = null;
  PFNGLISPROGRAMPROC glIsProgram = null;
  PFNGLUSEPROGRAMPROC glUseProgram = null;
  PFNGLUNIFORM1FVPROC glUniform1fv = null;
  PFNGLUNIFORM2FVPROC glUniform2fv = null;
  PFNGLUNIFORM3FVPROC glUniform3fv = null;
  PFNGLUNIFORM4FVPROC glUniform4fv = null;
  PFNGLUNIFORM1IVPROC glUniform1iv = null;
  PFNGLUNIFORM2IVPROC glUniform2iv = null;
  PFNGLUNIFORM3IVPROC glUniform3iv = null;
  PFNGLUNIFORM4IVPROC glUniform4iv = null;
  PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = null;
  PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = null;
  PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = null;
  PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = null;
  PFNGLUNIFORM1IPROC glUniform1i = null;
  PFNGLGETUNIFORMFVPROC glGetUniformfv = null;
} gles;

//PFNGLCREATEPROGRAMPROC           glesCreateProgram = null;

static GLint*         g_egl_compressed_format = null;
static GLint          g_egl_num_compressed_format = 0;

static bool g_supp_depth_tex   = false;
static bool g_supp_depth_tex24 = false;
static bool g_supp_depth_tex32 = false;
static bool g_supp_stencil_tex = false;
static bool g_supp_depcil_tex  = false;

static void __onDebugMessage(GLenum source,GLenum type,GLint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam)
{
  kgm_log() << "EGL Debug log: " << message << "\n";
}

kgmGLES::kgmGLES(kgmWindow *wnd)
{
  if(!wnd)
  {
    m_error = 1;

    return;
  }

  this->m_wnd = wnd;

  #ifdef DEBUG
  kgm_log() << "init GLES2.\n";
  #endif

  if (!m_egl.open("libEGL.so"))
  {
    m_error = 1;

    return;
  }

  egl.eglGetProcAddress = (PFNEGLGETPROCADDRESSPROC) m_egl.get("eglGetProcAddress");

  if (egl.eglGetProcAddress == null)
  {
    m_error = 1;

    return;
  }

  #define EGLEXT_FN(func)  egl.func = (typeof( egl.func )) egl.eglGetProcAddress(#func)

  EGLEXT_FN(eglBindAPI);
  EGLEXT_FN(eglDebugMessageCallbackKHR);
  EGLEXT_FN(eglGetDisplay);
  EGLEXT_FN(eglInitialize);
  EGLEXT_FN(eglChooseConfig);
  EGLEXT_FN(eglCreateWindowSurface);
  EGLEXT_FN(eglCreateContext);
  EGLEXT_FN(eglMakeCurrent);
  EGLEXT_FN(eglQueryContext);
  EGLEXT_FN(eglSwapInterval);
  EGLEXT_FN(eglQueryString);
  EGLEXT_FN(eglQuerySurface);
  EGLEXT_FN(eglDestroyContext);
  EGLEXT_FN(eglDestroySurface);
  EGLEXT_FN(eglTerminate);
  EGLEXT_FN(eglSwapBuffers);
  EGLEXT_FN(eglGetCurrentContext);
  EGLEXT_FN(eglGetError);

  #define GLESEXT_FN(func)  gles.func = (typeof( gles.func )) egl.eglGetProcAddress(#func)
  GLESEXT_FN(glGetString);
  GLESEXT_FN(glEnable);
  GLESEXT_FN(glDisable);
  GLESEXT_FN(glGetError);
  GLESEXT_FN(glGetIntegerv);
  GLESEXT_FN(glDepthFunc);
  GLESEXT_FN(glCullFace);
  GLESEXT_FN(glColorMask);
  GLESEXT_FN(glClearDepthf);
  GLESEXT_FN(glClearStencil);
  GLESEXT_FN(glClearColor);
  GLESEXT_FN(glClear);
  GLESEXT_FN(glFlush);
  GLESEXT_FN(glBlendEquation);
  GLESEXT_FN(glViewport);
  GLESEXT_FN(glBlendFunc);
  GLESEXT_FN(glDepthMask);
  GLESEXT_FN(glBindTexture);
  GLESEXT_FN(glGenTextures);
  GLESEXT_FN(glTexParameteri);
  GLESEXT_FN(glTexImage2D);
  GLESEXT_FN(glDeleteTextures);
  GLESEXT_FN(glActiveTexture);
  GLESEXT_FN(glGenFramebuffers);
  GLESEXT_FN(glBindFramebuffer);
  GLESEXT_FN(glGenRenderbuffers);
  GLESEXT_FN(glBindRenderbuffer);
  GLESEXT_FN(glRenderbufferStorage);
  GLESEXT_FN(glFramebufferRenderbuffer);
  GLESEXT_FN(glCheckFramebufferStatus);
  GLESEXT_FN(glFramebufferTexture2D);
  GLESEXT_FN(glDeleteRenderbuffers);
  GLESEXT_FN(glDeleteFramebuffers);
  GLESEXT_FN(glScissor);
  GLESEXT_FN(glBindBuffer);
  GLESEXT_FN(glGetAttribLocation);
  GLESEXT_FN(glVertexAttribPointer);
  GLESEXT_FN(glEnableVertexAttribArray);
  GLESEXT_FN(glDrawElements);
  GLESEXT_FN(glDrawArrays);
  GLESEXT_FN(glDisableVertexAttribArray);
  GLESEXT_FN(glGenBuffers);
  GLESEXT_FN(glDeleteBuffers);
  GLESEXT_FN(glCreateShader);
  GLESEXT_FN(glShaderSource);
  GLESEXT_FN(glCompileShader);
  GLESEXT_FN(glGetShaderiv);
  GLESEXT_FN(glGetShaderInfoLog);
  GLESEXT_FN(glDeleteShader);
  GLESEXT_FN(glAttachShader);
  GLESEXT_FN(glLinkProgram);
  GLESEXT_FN(glGetProgramiv);
  GLESEXT_FN(glGetProgramInfoLog);
  GLESEXT_FN(glCreateProgram);
  GLESEXT_FN(glDeleteProgram);
  GLESEXT_FN(glValidateProgram);
  GLESEXT_FN(glIsProgram);
  GLESEXT_FN(glUseProgram);
  GLESEXT_FN(glBindAttribLocation);
  GLESEXT_FN(glUniform1fv);
  GLESEXT_FN(glUniform2fv);
  GLESEXT_FN(glUniform3fv);
  GLESEXT_FN(glUniform4fv);
  GLESEXT_FN(glUniform1iv);
  GLESEXT_FN(glUniform2iv);
  GLESEXT_FN(glUniform3iv);
  GLESEXT_FN(glUniform4iv);
  GLESEXT_FN(glUniformMatrix2fv);
  GLESEXT_FN(glUniformMatrix3fv);
  GLESEXT_FN(glUniformMatrix4fv);
  GLESEXT_FN(glGetUniformLocation);
  GLESEXT_FN(glUniform1i);
  GLESEXT_FN(glGetUniformfv);

  EGLint    format;
  EGLint    numConfigs;
  EGLConfig config;

  if (!egl.eglBindAPI(EGL_OPENGL_ES_API))
  {
    kgm_log() << "Error: Unable Bind gles api, error is " << (s32) egl.eglGetError() << ".\n";

    m_error = 1;

    return;
  }

  m_display = EGL_NO_DISPLAY;
  m_context = EGL_NO_CONTEXT;
  m_surface = EGL_NO_SURFACE;

  //m_display = eglGetDisplay((EGLNativeDisplayType) wnd->m_dpy);  //eglGetDisplay(EGL_DEFAULT_DISPLAY);
  m_display = egl.eglGetDisplay(EGL_DEFAULT_DISPLAY);

  if (m_display == EGL_NO_DISPLAY)
  {
    kgm_log() << "Error: Got no EGL display.\n";

    m_error = 1;

    return;
  }

  EGLint egl_version_major = 0, egl_version_minor = 0;

  egl.eglInitialize(m_display, &egl_version_major, &egl_version_minor);

  kgm_log() << "EGL: Version: " << egl_version_major << "." << egl_version_minor << ".\n";

  EGLint egl_config_constraints[] = {
    EGL_COLOR_BUFFER_TYPE,     EGL_RGB_BUFFER,
    EGL_BUFFER_SIZE,           32,
    EGL_RED_SIZE,              8,
    EGL_GREEN_SIZE,            8,
    EGL_BLUE_SIZE,             8,
    EGL_ALPHA_SIZE,            8,

    EGL_DEPTH_SIZE,            24,
    EGL_STENCIL_SIZE,          8,

    EGL_SAMPLE_BUFFERS,        0,
    EGL_SAMPLES,               0,

    EGL_SURFACE_TYPE,          EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE,       EGL_OPENGL_ES2_BIT,

    EGL_NONE,
  };

  EGLConfig egl_conf;
  EGLint num_config;

  if (!egl.eglChooseConfig(m_display, egl_config_constraints, &egl_conf, 1, &num_config))
  {
    kgm_log() << "Error: Failed to choose config (eglError: " <<  egl.eglGetError() << ".\n";
    m_error = 1;

    return ;
  }

  kgm_log() << "Choose config count: " <<  num_config << ".\n";

  if (num_config != 1)
  {
    kgm_log() <<"Error: Didn't get exactly one config, but " << num_config << ".\n";
    m_error = 1;

    return;
  }

  const EGLint egl_surface_attribs[] = {
    EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
    EGL_NONE,
  };

  m_surface = egl.eglCreateWindowSurface(m_display, egl_conf, wnd->m_wnd, egl_surface_attribs);

  if (m_surface == EGL_NO_SURFACE)
  {
    kgm_log() << "Unable to create EGL surface (eglError: " <<  egl.eglGetError() << ").\n";
    m_error = 1;

    return;
  }

  EGLint ctxattr[] =
  {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE,
  };

  m_context = egl.eglCreateContext(m_display, egl_conf, EGL_NO_CONTEXT, ctxattr);

  if (m_context == EGL_NO_CONTEXT)
  {
    kgm_log() << "Error: Unable to create EGL context (eglError: " << egl.eglGetError() << ").\n";
    m_error = 1;

    return;
  }

  if (!egl.eglMakeCurrent(m_display, m_surface, m_surface, m_context))
  {
    kgm_log() << "Error: Unable to make EGL context current (eglError: " << egl.eglGetError() << ").\n";
    m_error = 1;

    return;
  }

  EGLint queriedRenderBuffer;
  if (egl.eglQueryContext(m_display, m_context, EGL_RENDER_BUFFER, &queriedRenderBuffer))
  {
    switch (queriedRenderBuffer)
    {
    case EGL_SINGLE_BUFFER:
      kgm_log() << "Render Buffer: EGL_SINGLE_BUFFER\n";
      break;
    case EGL_BACK_BUFFER:
      kgm_log() << "Render Buffer: EGL_BACK_BUFFER\n";
      break;
    case EGL_NONE:
      kgm_log() << "Render Buffer: EGL_NONE\n";
      break;
    default:
      kgm_log() << "Render Buffer: unknown value " << queriedRenderBuffer << "\n";
      break;
    }
  }
  else
  {
    kgm_log() << "Failed to query EGL_RENDER_BUFFER: " << egl.eglGetError() << "\n";
  }

  if (!egl.eglSwapInterval(m_display, 1))
  {
    kgm_log() << "eglSwapInterval failed: " << egl.eglGetError() << "\n";
  }
  else
  {
    kgm_log() << "Set swap interval\n";
  }

  //eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format);
  //ANativeWindow_setBuffersGeometry((ANativeWindow*)kgmApp::application()->m_nativeWindow, 0, 0, format);
  //surface = eglCreateWindowSurface(display, config, (ANativeWindow*)kgmApp::application()->m_nativeWindow, NULL);
  //m_context = eglCreateContext(m_display, config, NULL, NULL);

  //if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE)
  //{
//#ifdef DEBUG
    //kgm_log() << "kgmEngine: Unable to eglMakeCurrent \n";
//#endif

    //return;
  //}

  int w = 0, h = 0;

  egl.eglQuerySurface(m_display, m_surface, EGL_WIDTH, &w);
  egl.eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &h);

  kgm_log() << "GLES2 surface w: " << w << ", h: " << h << "\n";


#ifdef DEBUG
  kgm_log() << "GLES Version: " << (char*) gles.glGetString(GL_VERSION) << "\n";
  kgm_log() << "GLSL Version: " << (char*) gles.glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
  kgm_log() << "GLES Vendor: " << (char*) gles.glGetString(GL_VENDOR) << "\n";
  kgm_log() << "GLES Render: " << (char*) gles.glGetString(GL_RENDERER) << "\n";
#endif

  const GLubyte* ext = gles.glGetString(GL_EXTENSIONS);

  kgm_log() << "GLES Extentions: " << (s8*) ext << "\n";

  if (!ext)
  {
    kgm_log() << "Error: No gles extentions.\n";

    return;
  }

  if(strstr((char*)ext, "GL_ARB_shader_objects"))
  {
    m_error = 2;

    return;
  }

  if(strstr((char*)ext, "GL_ARB_framebuffer_object"))
  {
    m_error = 3;

    return;
  }

  if (strstr((s8*) ext, "GL_OES_depth_texture"))
  {
    g_supp_depth_tex = true;

    if (strstr((s8*) ext, "GL_OES_depth24"))
    {
      g_supp_depth_tex24 = true;
    }

    if (strstr((s8*) ext, "GL_OES_depth32"))
    {
      g_supp_depth_tex32 = true;
    }
  }

  if (strstr((s8*) ext, "GL_OES_texture_stencil8"))
  {
    g_supp_stencil_tex = true;
  }

  if (strstr((s8*) ext, "GL_OES_packed_depth_stencil"))
  {
    g_supp_depcil_tex = true;
  }

#ifdef DEBUG

  //eglDebugMessageCallbackKHR = (PFNGLDEBUGMESSAGECALLBACKKHRPROC) eglGetProcAddress("glDebugMessageCallbackKHR");

  if (egl.eglDebugMessageCallbackKHR != null)
  {
    kgm_log() << "Enabling GL Debuging.\n";

    gles.glEnable(GL_DEBUG_OUTPUT_KHR);
    gles.glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);

    egl.eglDebugMessageCallbackKHR( (GLDEBUGPROCKHR) __onDebugMessage, this);

    kgm_log() << "Enabling GL Debuging status " << gles.glGetError() << ".\n";
  }

#endif

  //glesCreateProgram = (PFNGLCREATEPROGRAMPROC) egl.eglGetProcAddress("glCreateProgram");

#ifdef GL_NUM_COMPRESSED_TEXTURE_FORMATS
  gles.glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &g_egl_num_compressed_format);

  g_egl_compressed_format = (GLint*)kgm_alloc(g_egl_num_compressed_format * sizeof(GLint));

  gles.glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, g_egl_compressed_format);

  for(int i = 0; i < g_egl_num_compressed_format; i++)
  {
    int k = g_egl_compressed_format[i];
    int a = 0;
  }
#endif

  GLint max_image_units = 0;

  gles.glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_image_units);

  kgm_log() << "GLES Max texture units: " << (s32) max_image_units << "\n";

  //init local values
  gles.glEnable(GL_DEPTH_TEST);
  gles.glDepthFunc(GL_LEQUAL);
  gles.glEnable(GL_CULL_FACE);
  gles.glCullFace(GL_BACK);

  m_renderbuffer = 0;

  m_min_filter = GL_LINEAR;
  m_mag_filter = GL_LINEAR;

  kgm_log() << "GLES Info: egl client apis " << (s8*) egl.eglQueryString(m_display, EGL_CLIENT_APIS) << "\n";
  kgm_log() << "GLES Info: egl vendor " << (s8*) egl.eglQueryString(m_display, EGL_VENDOR) << "\n";
  kgm_log() << "GLES Info: egl version " << (s8*) egl.eglQueryString(m_display, EGL_VERSION) << "\n";

  GLint param = 0;

  if (egl.eglQueryContext(m_display, m_context, EGL_CONTEXT_CLIENT_VERSION, &param))
  {
    kgm_log() << "gcGenShader: Context client version " << (s32) param << "\n";
  }

  kgm_log() << "GLES Initialized.\n";
}

kgmGLES::~kgmGLES()
{
#ifdef GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB
  if(g_compressed_format)
    kgm_free(g_compressed_format);
#endif

  if (m_display != EGL_NO_DISPLAY)
  {
    egl.eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    if (m_context != EGL_NO_CONTEXT)
    {
      egl.eglDestroyContext(m_display, m_context);
    }

    if (m_surface != EGL_NO_SURFACE)
    {
      egl.eglDestroySurface(m_display, m_surface);
    }

    egl.eglTerminate(m_display);
  }

  m_display = EGL_NO_DISPLAY;
  m_context = EGL_NO_CONTEXT;
  m_surface = EGL_NO_SURFACE;

#ifdef DEBUG
  kgm_log() << "kgmGLES::~kgmGLES.\n";
#endif
}

void kgmGLES::gcSet(u32 param, void* value)
{
  u32 tmp = 0;
  switch(param)
  {
  case gcpar_blending:
    if(value)
      gles.glEnable(GL_BLEND);
    else
      gles.glDisable(GL_BLEND);
    break;
  case gcpar_culling:
    if(value)
      gles.glEnable(GL_CULL_FACE);
    else
      gles.glDisable(GL_CULL_FACE);
    break;
  case gcpar_textures:
    if(value)
      gles.glEnable(GL_TEXTURE_2D);
    else
      gles.glDisable(GL_TEXTURE_2D);
    break;
  case gcpar_depth:
    if(value)
      gles.glEnable(GL_DEPTH_TEST);
    else
      gles.glDisable(GL_DEPTH_TEST);
    break;
#ifdef GL_ALPHA_TEST
  case gcpar_alpha:
    if(value)
      gles.glEnable(GL_ALPHA_TEST);
    else
      gles.glDisable(GL_ALPHA_TEST);
    break;
#endif
  case gcpar_stencil:
    if(value)
      gles.glEnable(GL_STENCIL_TEST);
    else
      gles.glDisable(GL_STENCIL_TEST);
    break;
  case gcpar_colormask:
  {
    char* c = (char*)&value;
    GLboolean r = (c[0])?(GL_TRUE):(GL_FALSE),
        g = (c[1])?(GL_TRUE):(GL_FALSE),
        b = (c[2])?(GL_TRUE):(GL_FALSE),
        a = (c[3])?(GL_TRUE):(GL_FALSE);
    gles.glColorMask(r, g, b, a);
  }
    break;
  case gcpar_clipplane:
  {
  }
    break;
  case gcpar_texfltmag:
    m_mag_filter = gl_enum((u32)(size_t)value);
    break;
  case gcpar_texfltmin:
    m_min_filter = gl_enum((u32)(size_t)value);
    break;
  case gcpar_texfilter:
  {
    TexFilter *filter = (TexFilter *) value;
    m_mag_filter = gl_enum(filter->mag);
    m_min_filter = gl_enum(filter->min);
  }
    break;
  case gcpar_cubemapside:
    m_cubemapside = *(u32*) value;
  }
}

void kgmGLES::gcGet(u32 param, void* value)
{
  switch(param)
  {
  case gcsup_shaders:
    *((u32*)value) = 1;
    break;
  case gcsup_fbuffers:
    *((u32*)value) = 1;
    break;
  case gc_rdev:
    //*((const char*)value) = "opengl";
    break;
  case gcpar_viewrect:
    ((s32*)value)[0] = m_viewport[0];
    ((s32*)value)[1] = m_viewport[1];
    ((s32*)value)[2] = m_viewport[2];
    ((s32*)value)[3] = m_viewport[3];
    break;
  }
}

u32 kgmGLES::gcError()
{
  return m_error;
}

void kgmGLES::gcClear(u32 flag, u32 col, float depth, u32 sten)
{
  GLbitfield cl = 0;

  if(flag & gcflag_color)
  {
    cl |= GL_COLOR_BUFFER_BIT;
    float inv = 1.0f / 255.0f;
    float a = inv * ((col & 0xff000000) >> 24),
        r = inv * ((col & 0x00ff0000) >> 16),
        g = inv * ((col & 0x0000ff00) >> 8),
        b = inv *  (col & 0x000000ff);

    gles.glClearColor(r, g, b, a);
  }

  if(flag & gcflag_depth)
  {
    cl |= GL_DEPTH_BUFFER_BIT;
    gles.glClearDepthf(depth);
  }

  if(flag & gcflag_stencil)
  {
    cl |= GL_STENCIL_BUFFER_BIT;
    gles.glClearStencil(sten);
  }

  gles.glClear(cl);
  gles.glFlush();
}

void kgmGLES::gcBegin()
{
}

void kgmGLES::gcEnd()
{
}

void kgmGLES::gcRender()
{
  egl.eglSwapBuffers(m_display, m_surface);
}

void kgmGLES::gcResize(u32 width, u32 height)
{

}

void kgmGLES::gcSetViewport(int x, int y, int w, int h, float n, float f)
{
  m_viewport[0] = x;
  m_viewport[1] = y;
  m_viewport[2] = w;
  m_viewport[3] = h;

  gles.glViewport(x, y, w, h);
}


//BLEND
void  kgmGLES::gcBlend(bool en, u32 eq, u32 fnsrc, u32 fndst)
{
  if(!en)
  {
    gles.glDisable(GL_BLEND);

    return;
  }

  switch(eq)
  {
  case gcblend_eqadd:
    gles.glBlendEquation(GL_FUNC_ADD);
    break;
  case gcblend_eqsub:
    gles.glBlendEquation(GL_FUNC_SUBTRACT);
    break;
  case gcblend_eqmin:
    gles.glBlendEquation(GL_MIN);
    break;
  case gcblend_eqmax:
    gles.glBlendEquation(GL_MAX);
    break;
  case gcblend_eqrsub:
    gles.glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
    break;
  default:
    gles.glBlendEquation(GL_FUNC_ADD);
  }

  gles.glBlendFunc(gl_enum(fnsrc), gl_enum(fndst));
  gles.glEnable(GL_BLEND);
}

//CULL
void kgmGLES::gcCull(u32 mode)
{
  if(!mode)
  {
    gles.glDisable(GL_CULL_FACE);

    return;
  }

  switch(mode)
  {
  case gccull_back:
    gles.glCullFace(GL_BACK);
    break;
  case gccull_front:
    gles.glCullFace(GL_FRONT);
    break;
  }

  gles.glEnable(GL_CULL_FACE);
}

//DEPTH
void kgmGLES::gcDepth(bool depth, bool mask, u32 mode)
{
  if(depth)
    gles.glEnable(GL_DEPTH_TEST);
  else
    gles.glDisable(GL_DEPTH_TEST);

  if(mask)
    gles.glDepthMask(GL_TRUE);
  else
    gles.glDepthMask(GL_FALSE);

  gles.glDepthFunc(gl_enum(mode));
}

//TEXTURE
gchandle kgmGLES::gcGenTexture(void *pd, u32 w, u32 h, u32 fmt, u32 type)
{
  u32    tex     = 0;
  GLenum pic_fmt = 0;
  u32    int_fmt = 0;
  u32    tex_typ = 0;
  u32    bpc     = 1;

#ifdef DEBUG
  kgm_log() << "gcGenTexture " << (s32)w << " " << (s32)h << " " << (s32)fmt << "\n";
#endif

  switch(fmt)
  {
  case gctex_fmt8:
    pic_fmt = GL_ONE;
    int_fmt = GL_RGB;
    tex_typ = GL_UNSIGNED_BYTE;
    break;
  case gctex_fmt16:
    pic_fmt = GL_RGB;
    int_fmt = GL_RGB;
    tex_typ = GL_UNSIGNED_SHORT_5_6_5;
    bpc = 2;
    break;
  case gctex_fmt24:
    pic_fmt = GL_RGB;
    int_fmt = GL_RGB;
    tex_typ = GL_UNSIGNED_BYTE;
    bpc = 3;
    break;
  case gctex_fmt32:
    pic_fmt = GL_RGBA;
    int_fmt = GL_RGBA;
    tex_typ = GL_UNSIGNED_BYTE;
    bpc = 4;
    break;
  case gctex_fmtdepth:
    if (g_supp_depth_tex)
    {
      pic_fmt = GL_DEPTH_COMPONENT;
      int_fmt = GL_DEPTH_COMPONENT;
      tex_typ = GL_UNSIGNED_INT;
    }
    else
    {
      pic_fmt = GL_RGBA;
      int_fmt = GL_RGBA;
      tex_typ = GL_UNSIGNED_BYTE;
    }
    break;
  case gctex_fmtdepcil:
    if (g_supp_depcil_tex)
    pic_fmt = GL_DEPTH_STENCIL_OES;
    int_fmt = GL_DEPTH_STENCIL_OES;
    tex_typ = GL_UNSIGNED_INT;
    break;
  }

  u8* cnt = null;

  switch(type)
  {
  case gctype_tex2d:
    gles.glGenTextures(1, &tex);
    gles.glBindTexture(GL_TEXTURE_2D, tex);
    break;
  case gctype_texcube:
    gles.glGenTextures(1, &tex);
    gles.glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
    break;
  }

  switch(type)
  {
  case gctype_tex2d:
    if(fmt == gctex_fmtdepth)
    {
      gles.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      gles.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
      gles.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
      gles.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);

      gles.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      gles.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    gles.glTexImage2D(GL_TEXTURE_2D, 0, int_fmt, w, h, 0, pic_fmt, tex_typ, pd);

#ifdef DEBUG
    if (gles.glGetError() != GL_NO_ERROR)
      kgm_log() << "Error: Cannot create 2D texture. eid=" << (s32) gles.glGetError() << ".\n";
#endif
    break;
  case gctype_texcube:
    cnt = (u8*) kgm_alloc(bpc * w * h);
    memset(cnt, 0xff, bpc * w * h);

    gles.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gles.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gles.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gles.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gles.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R_OES, GL_CLAMP_TO_EDGE);
    gles.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    gles.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    gles.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    gles.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    gles.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    gles.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    kgm_free(cnt);
    break;
  }

  switch(type)
  {
  case gctype_texcube:
    gles.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    break;
  default:
    gles.glBindTexture(GL_TEXTURE_2D, 0);
  }

#ifdef DEBUG
  kgm_log() << "Gen texture: " << (s32) tex << "\n";
#endif

  return (gchandle) ((size_t) tex);
}

void kgmGLES::gcFreeTexture(gchandle t)
{
  if(!t)
    return;

  u32 tex = (u32) (size_t) t;

  if(t)
    gles.glDeleteTextures(1, &tex);


#ifdef DEBUG
  kgm_log() << "Free texture: " << (s32) tex << "\n";
#endif
}

void kgmGLES::gcSetTexture(u32 stage, gchandle t)
{
  if(!t)
  {
    gles.glActiveTexture(GL_TEXTURE0 + stage);
    gles.glBindTexture(GL_TEXTURE_2D, 0);

    if (gles.glGetError() == GL_INVALID_OPERATION)
    {
      gles.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

      if (gles.glGetError() != GL_NO_ERROR)
      {
        int k = 0;
      }
    }

    return;
  }

  gles.glActiveTexture(GL_TEXTURE0 + stage);
  gles.glBindTexture(GL_TEXTURE_2D, (u32) (size_t) t);

  if (gles.glGetError() == GL_INVALID_OPERATION)
  {
    gles.glBindTexture(GL_TEXTURE_CUBE_MAP, (u32) (size_t) t);

    if (gles.glGetError() != GL_NO_ERROR)
    {
      int k = 0;
    }
  }
}

// TARGET
gchandle kgmGLES::gcGenTarget(u32 w, u32 h, bool d, bool s)
{
  u32 buffer = 0;
  gles.glGenFramebuffers(1, &buffer);
  gles.glBindFramebuffer(GL_FRAMEBUFFER, buffer);

  u32 color = 0;

  gles.glGenRenderbuffers(1, &color);
  gles.glBindRenderbuffer( GL_RENDERBUFFER, (GLuint)color);
  gles.glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA, w, h );
  gles.glBindRenderbuffer( GL_RENDERBUFFER, 0 );

  u32 depth = 0;
  u32 stencil = 0;

  if (d)
  {
    gles.glGenRenderbuffers(1, &depth);
    gles.glBindRenderbuffer(GL_RENDERBUFFER, depth);

    if (g_supp_depcil_tex)
    {
      gles.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, w, h);
      gles.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
      gles.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth);
      stencil = depth;
    }
    else
    {
      gles.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
      gles.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    }
  }

  if (!stencil)
  {
    gles.glGenRenderbuffers(1, &stencil);
    gles.glBindRenderbuffer(GL_RENDERBUFFER, stencil);
    gles.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil);
    gles.glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, w, h);
  }

#ifdef DEBUG
  GLenum status;
  status = gles.glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if(status != GL_FRAMEBUFFER_COMPLETE)
  {
    kgm_log() << "Error: Cannot use framebuffer object!\n";
    return null;
  }
#endif

  gles.glBindFramebuffer(GL_FRAMEBUFFER, 0);
  gles.glBindRenderbuffer(GL_RENDERBUFFER, 0);

  RenderBuffer* rb = (RenderBuffer*) kgm_alloc(sizeof(RenderBuffer));

  rb->frame   = buffer;
  rb->color   = color;
  rb->depth   = depth;
  rb->stencil = stencil;
  rb->width   = w;
  rb->height  = h;
  rb->cmside  = 0;

  return (gchandle) rb;
}

bool kgmGLES::gcTexTarget(gchandle tar, gchandle tex, u32 type)
{

  RenderBuffer* rb = static_cast<RenderBuffer*>(tar);

  if(!rb)
    return false;

  gles.glBindFramebuffer(GL_FRAMEBUFFER, rb->frame);

  switch (type)
  {
  case gctype_texdepth:
    gles.glBindRenderbuffer(GL_RENDERBUFFER, rb->depth);
    gles.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,  (GLint) (size_t) tex, 0);
    break;
  case gctype_tex2d:
    gles.glBindRenderbuffer(GL_RENDERBUFFER, rb->color);
    gles.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLint) (size_t) tex, 0);
    break;
  case gctype_texcube:
    gles.glBindRenderbuffer(GL_RENDERBUFFER, rb->color);
    gles.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_cubemapside, (GLint) (size_t) tex, 0);

    ++rb->cmside;

    if (rb->cmside > 5)
      rb->cmside = 0;
    break;
  default:
    gles.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLint) (size_t) tex, 0);
  };

#ifdef DEBUG
  GLenum status = gles.glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE) {
    kgm_log() << "Error: Attaching texture to framebuffer not completed.\n";
    kgm_log() << "Error: " << (s32)status << ".\n";
  }
#endif

  gles.glBindFramebuffer(GL_FRAMEBUFFER, 0);
  gles.glBindRenderbuffer(GL_RENDERBUFFER, 0);

  return true;
}

void kgmGLES::gcFreeTarget(gchandle t)
{
  RenderBuffer* rb = static_cast<RenderBuffer*>(t);

  if(!rb)
    return;

  if (rb->color)
    gles.glDeleteRenderbuffers(1, &rb->color);

  if (rb->stencil != rb->depth)
    gles.glDeleteRenderbuffers(1, &rb->stencil);

  if (rb->depth)
    gles.glDeleteRenderbuffers(1, &rb->depth);

  if (rb->frame)
    gles.glDeleteFramebuffers(1, &rb->frame);

  kgm_free(rb);
}

void kgmGLES::gcSetTarget(gchandle t)
{
  RenderBuffer* rb = (RenderBuffer*) t;

  if(!rb || !rb->frame)
  {
    gles.glBindFramebuffer(GL_FRAMEBUFFER, 0);
    gles.glBindRenderbuffer(GL_RENDERBUFFER, 0);

    m_renderbuffer = null;

    return;
  }

  gles.glBindFramebuffer(GL_FRAMEBUFFER, rb->frame);

  m_renderbuffer = rb->frame;

#ifdef DEBUG
  GLenum err = gles.glGetError();

  if (err != GL_NO_ERROR)
    kgm_log() << "Error: Cannot set framebuffer, error is " << err << ".\n";
#endif
}

//STENCIL
void kgmGLES::gcStencil(bool en, u32 func, u32 mask, u32 ref,
               u32 fail, u32 zfail, u32 zpass)
{

}

//SCISSOR
void kgmGLES::gcScissor(bool on, int x, int y, int w, int h)
{
  gles.glScissor(x, y, w, h);

  if (on)
    gles.glEnable(GL_SCISSOR_TEST);
  else
    gles.glDisable(GL_SCISSOR_TEST);
}

//Drawing
void kgmGLES::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt,
                    u32 i_size, u32 i_cnt, void *i_pnt)
{
  if(!v_pnt)
    return;

  GLint program = 0;

  gles.glGetIntegerv(GL_CURRENT_PROGRAM, &program);

  if (program == 0 || program != m_shader)
  {
    return;
  }

  unsigned char *pM = (unsigned char*)v_pnt;
  unsigned int  uv_size = sizeof(float) * 2;

  int ah = 0;

  gles.glBindBuffer(GL_ARRAY_BUFFER, 0);
  gles.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(v_fmt & gcv_xyz)
  {
    ah = gles.glGetAttribLocation(program, "a_Vertex");

    if(ah != -1)
    {
      gles.glVertexAttribPointer(ah, 3, GL_FLOAT, GL_FALSE, v_size, pM);
      gles.glEnableVertexAttribArray(ah);
    }

    pM += (sizeof(float) * 3);
  }

  if(v_fmt & gcv_nor)
  {
    ah = gles.glGetAttribLocation(program, "a_Normal");

    if(ah != -1)
    {
      gles.glEnableVertexAttribArray(ah);
      gles.glVertexAttribPointer(ah, 3, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (sizeof(float)*3);
  }

  if(v_fmt & gcv_col)
  {
    ah = gles.glGetAttribLocation(program, "a_Color");

    if(ah != -1)
    {
      gles.glEnableVertexAttribArray(ah);
      gles.glVertexAttribPointer(ah, 4, GL_FLOAT, GL_TRUE, v_size, pM);
    }

    pM += (sizeof(float) * 4);
  }

  if(v_fmt & gcv_uv0)
  {
    ah = gles.glGetAttribLocation(program, "a_UV");

    if(ah != -1)
    {
      gles.glEnableVertexAttribArray(ah);
      gles.glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_uv1)
  {
    ah = gles.glGetAttribLocation(program, "a_UV2");
    if(ah != -1)
    {
      gles.glEnableVertexAttribArray(ah);
      gles.glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_uv2)
  {
    ah = gles.glGetAttribLocation(program, "a_UV3");

    if(ah != -1)
    {
      gles.glEnableVertexAttribArray(ah);
      gles.glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_bn0)
  {
    ah = gles.glGetAttribLocation(program, "a_Weights");
    gles.glEnableVertexAttribArray(ah);
    gles.glVertexAttribPointer(ah, 4, GL_FLOAT, GL_FALSE, v_size, pM);
    pM += (4 * sizeof(float));
    ah = gles.glGetAttribLocation(program, "a_Indices");
    gles.glEnableVertexAttribArray(ah);
    gles.glVertexAttribPointer(ah, 4, GL_FLOAT, GL_FALSE, v_size, pM);
    pM += (4 * sizeof(float));
  }

  if(i_pnt && i_cnt)
  {
    switch(i_size)
    {
    case 4:
      gles.glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_INT, i_pnt);
    break;
    default:
      gles.glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_SHORT,i_pnt);
    }
  }
  else
  {
    gles.glDrawArrays(gl_enum(pmt), 0, v_cnt);
  }

  gles.glDisableVertexAttribArray(0);
  gles.glDisableVertexAttribArray(1);
  gles.glDisableVertexAttribArray(2);
  gles.glDisableVertexAttribArray(3);
  gles.glDisableVertexAttribArray(4);
  gles.glDisableVertexAttribArray(5);
}

//VERTEX & INDEX BUFFER
gchandle kgmGLES::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize)
{
  VBO* vbo = new VBO;
  memset(vbo, 0, sizeof(VBO));

  if(vdata && vsize)
  {
    gles.glGenBuffers(1, &vbo->vb);
    gles.glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
    gles.glBufferData(GL_ARRAY_BUFFER, vsize, vdata, GL_STATIC_DRAW);
    vbo->vsize = vsize;
  }

  if(idata && isize)
  {
    gles.glGenBuffers(1, &vbo->ib);
    gles.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);
    gles.glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, idata, GL_STATIC_DRAW);
    vbo->isize = isize;
  }

  gles.glBindBuffer(GL_ARRAY_BUFFER, 0);
  gles.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return (gchandle) vbo;
}

void  kgmGLES::gcFreeVertexBuffer(gchandle b)
{
  VBO* vbo = (VBO*)b;

  if(!vbo)
    return;

  if(vbo->vb)
    gles.glDeleteBuffers(1, &vbo->vb);

  if(vbo->ib)
    gles.glDeleteBuffers(1, &vbo->ib);

  delete vbo;
}

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

void  kgmGLES::gcDrawVertexBuffer(gchandle b, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt,
                                 u32 isize, u32 icnt, u32 ioff)
{
  VBO* vbo = (VBO*)b;
  size_t offset = 0;
  u32 uvt = 2;

  int al = 0;

  if (!vbo)
    return;

   GLint program = 0;

  gles.glGetIntegerv(GL_CURRENT_PROGRAM, &program);

  if (program == 0 || program != m_shader)
  {
    return;
  }

  //glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

  if(vbo->vb)
  {
    gles.glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
  }

  /*{
    struct V { float v[3], n[3], uv[2]; };

    V *v = (V*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

    if (v)
    {
      V *c = 0;

      for (int i = 0; i < vcnt; i++)
      {
        c = &v[i];
      }

      glUnmapBuffer(GL_ARRAY_BUFFER);
    }
  }*/

  if(vfmt & gcv_xyz)
  {
    al = gles.glGetAttribLocation(program, "a_Vertex");

    if(al != -1)
    {
      gles.glEnableVertexAttribArray(al);
      gles.glVertexAttribPointer(al, 3, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 3;
    }
  }

  if(vfmt & gcv_nor)
  {
    al = gles.glGetAttribLocation(program, "a_Normal");

    if(al != -1)
    {
      gles.glEnableVertexAttribArray(al);
      gles.glVertexAttribPointer(al, 3, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 3;
    }
  }

  if(vfmt & gcv_col)
  {
    al = gles.glGetAttribLocation(program, "a_Color");

    if(al != -1)
    {
      gles.glEnableVertexAttribArray(al);
      gles.glVertexAttribPointer(al, 4, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 4;
    }
  }

  if(vfmt & gcv_uv0)
  {
    al = gles.glGetAttribLocation(program, "a_UV");

    if(al != -1)
    {
      gles.glEnableVertexAttribArray(al);
      gles.glVertexAttribPointer(al, 2, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * uvt;
    }
  }

    if(vbo->ib && icnt)
  {
    //  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    gles.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);

    /*{
      struct V { short f[3]; };

      V *v = (V*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

      if (v)
      {
        V *c = 0;

        for (int i = 0; i < icnt; i++)
        {
          c = &v[i];
        }

        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      }
    }*/


    switch(isize)
    {
    case 4:
      //glIndexPointer(GL_UNSIGNED_INT, 0, (void*) (size_t) ioff);
      gles.glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_INT, BUFFER_OFFSET(ioff));
      //glDrawRangeElements (gl_enum(pmt), 0, vcnt - 1, icnt, GL_UNSIGNED_INT, ((char*)0L + ioff));
      break;
    default:
      //glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);
      gles.glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_SHORT, BUFFER_OFFSET(ioff));
      //glDrawRangeElements (gl_enum(pmt), ioff, ioff + icnt, icnt/3, GL_UNSIGNED_SHORT, (void*)0);
    }
  }
  else
  {
    gles.glDrawArrays(gl_enum(pmt), 0, vcnt);
  }

  gles.glBindBuffer( GL_ARRAY_BUFFER, 0);
  gles.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  //glDisableClientState(GL_VERTEX_ARRAY);
  //glDisableClientState(GL_NORMAL_ARRAY);
  //glDisableClientState(GL_COLOR_ARRAY);
  //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  //glPopClientAttrib();
}

// SHADERS

gchandle kgmGLES::gcGenShader(kgmArray<u8>& vsrc, kgmArray<u8>& fsrc)
{
  kgm_log() << "gcGenShader: Generating shader.\n";

  if ((egl.eglGetCurrentContext() == EGL_NO_CONTEXT) || (egl.eglGetCurrentContext() != m_context))
  {
    kgm_log() << "gcGenShader: Wrong EGL context.\n";

    return null;
  }

  kgmString vstr;

  kgm_log() << "gcGenShader: vshader length> " << vsrc.length() << ", fshader length> " << fsrc.length() << ".\n";

  vstr.alloc((char *) vsrc.data(), vsrc.length());

  kgm_log() << "gcGenShader: Allocate vshader string.\n";

  GLhandle prog = 0;
  GLhandle vshad = 0, fshad = 0;
  int stat[1] = {0};
  int size = 512;
  int gerr = 0;

#ifdef DEBUG
  char tbuf[512];
#endif

  //GL_VERTEX_SHADER
  if(vsrc.data() && vsrc.length())
  {
    const char *ptr = (const char*) vsrc.data();

    size = strlen(ptr);
    vshad = gles.glCreateShader(GL_VERTEX_SHADER);
    gles.glShaderSource(vshad, 1, (const GLchar**)&ptr, &size);
    gles.glCompileShader(vshad);

    GLint length;

    gles.glGetShaderiv(vshad, GL_INFO_LOG_LENGTH, &length);

    if (length)
    {
      char* buffer = (char*) kgm_alloc(length);
      gles.glGetShaderInfoLog(vshad, length, NULL, buffer);
      kgm_log() << "gcGenShader: Compile vshader log: " << buffer << "\n";
      kgm_free(buffer);
    }

    gles.glGetShaderiv(vshad, GL_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE)
    {
      kgm_log() << "gcGenShader: Compile vshader failed.\n";
      gles.glDeleteShader(vshad);
      vshad = 0;
    }
  }

  kgm_log() << "gcGenShader: Allocate fshader string.\n";

  //FRAGMENT_SHADER
  if(fsrc.data() && fsrc.length())
  {
    const char *ptr = (const char*) fsrc.data();

    size = strlen(ptr);
    fshad = gles.glCreateShader(GL_FRAGMENT_SHADER);
    gles.glShaderSource(fshad, 1, (const GLchar**)&ptr, NULL);
    gles.glCompileShader(fshad);

    GLint length;

    gles.glGetShaderiv(vshad, GL_INFO_LOG_LENGTH, &length);

    if (length)
    {
      char* buffer = (char*) kgm_alloc(length);
      gles.glGetShaderInfoLog(vshad, length, NULL, buffer);
      kgm_log() << "gcGenShader: Compile fshader log: " << buffer << "\n";
      kgm_free(buffer);
    }

    gles.glGetShaderiv(fshad, GL_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE)
    {
      kgm_log() << "gcGenShader: Compile fshader failed.\n";
      gles.glDeleteShader(fshad);
      fshad = 0;
    }
  }

  prog = gles.glCreateProgram();

  kgm_log() << "gcGenShader: Create program is " << (s32) prog << "\n";

  gerr = gles.glGetError();

  kgm_log() << "gcGenShader: Create program error is " << (s32) gerr << "\n";

  if(prog < 1 || GL_NO_ERROR != gerr)
  {
    int s = gerr;

#ifdef DEBUG
  kgm_log() << "gcGenShader: error is " << (s32)gerr << "\n";
#endif

    return null;
  }

#ifdef DEBUG
  kgm_log() << "gcGenShader: Shader program id is " << (s32) prog << "\n";
#endif

  kgm_log() << "gcGenShader: Attaching vshader " << (s32) vshad << " to program " << (s32) prog << "\n";

  gles.glAttachShader(prog, vshad);

  kgm_log() << "gcGenShader: Attaching vshader error " << (s32) gles.glGetError() << "\n";

  kgm_log() << "gcGenShader: Attaching fshader " << (s32) vshad << " to program " << (s32) prog << "\n";

  gles.glAttachShader(prog, fshad);

  kgm_log() << "gcGenShader: Attaching fshader error " << (s32) gles.glGetError() << "\n";

  //glBindAttribLocation((GLhandle)prog, 0, "g_Vertex");
  //glBindAttribLocation((GLhandle)prog, 1, "g_Normal");
  //glBindAttribLocation((GLhandle)prog, 2, "g_Color");
  //glBindAttribLocation((GLhandle)prog, 3, "g_UV");

  gles.glLinkProgram(prog);

  kgm_log() << "gcGenShader: Link program error: " << gles.glGetError() << "\n";

  gles.glGetProgramiv(prog, GL_LINK_STATUS, stat);

  if(stat[0] == GL_FALSE)
  {
#ifdef DEBUG
    GLint length = 0;

    gles.glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);

    kgm_log() << "gcGenShader: Link program log length: " << length << "\n";

    if (length)
    {
      char* buffer = (char*) kgm_alloc(length);
      gles.glGetProgramInfoLog(prog, length, NULL, buffer);
      kgm_log() << "gcGenShader: Link program log: " << buffer << "\n";
      kgm_free(buffer);
    }
#endif

    gles.glDeleteShader(vshad);
    gles.glDeleteShader(fshad);
    gles.glDeleteProgram(prog);

    return null;
  }

  gles.glValidateProgram(prog);

  kgm_log() << "gcGenShader: Validate program error: " << gles.glGetError() << "\n";

  gles.glGetProgramiv(prog, GL_VALIDATE_STATUS, stat);

  if(stat[0] == GL_FALSE)
  {
#ifdef DEBUG
    GLint length = 0;

    gles.glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);

    kgm_log() << "gcGenShader: Validate program log length: " << length << "\n";

    if (length)
    {
      char* buffer = (char*) kgm_alloc(length);
      gles.glGetProgramInfoLog(prog, length, NULL, buffer);
      kgm_log() << "gcGenShader: Validate program log: " << buffer << "\n";
      kgm_free(buffer);
    }
#endif

    gles.glDeleteShader(vshad);
    gles.glDeleteShader(fshad);
    gles.glDeleteProgram(prog);

    return null;
  }

  //glDetachShader(prog, vshad);
  //glDeleteShader(vshad);
  //glDetachShader(prog, fshad);
  //glDeleteShader(fshad);

  if (gles.glIsProgram(prog) == GL_FALSE)
  {
    kgm_log() << "gcGenShader: Is program error " << (s32) gles.glGetError() << ".\n";

    gles.glDeleteShader(vshad);
    gles.glDeleteShader(fshad);
    gles.glDeleteProgram(prog);

    return null;
  }

  kgm_log() << "gcGenShader: Shader program valid id is " << (s32) prog << ".\n";

  return (gchandle) (size_t) prog;
}

void kgmGLES::gcFreeShader(gchandle s)
{
  size_t shader = (size_t) s;

  if(shader)
  {
    kgm_log() << "gcFreeShader: Release shader program is " << (s32) shader << ".\n";

    gles.glDeleteProgram((GLhandle) shader);
  }
}

void kgmGLES::gcSetShader(gchandle s)
{
#ifdef DEBUG
  //kgm_log() << "gcSetShader: Shader program id is " << (s32) (size_t) s << "\n";
#endif

  if (s == 0)
  {
    gles.glUseProgram(0);
    m_shader = 0;

    return;
  }

  GLint prog = 0;

  gles.glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

  if (prog == (GLint) (size_t) s)
  {
    return;
  }

  if (gles.glIsProgram((GLuint) (size_t) s) != GL_TRUE)
  {
#ifdef DEBUG
    kgm_log() << "gcSetShader: Shader program " << (s32) (size_t) s << " is invalid as " << gles.glGetError() << ".\n";
#endif
    return;
  }

  gles.glUseProgram((GLuint) (size_t) s);

  m_shader = (GLhandle)(size_t) s;
}

void  kgmGLES::gcBindAttribute(void* s, int i, const char* attr)
{
  if(s)
  {
    gles.glBindAttribLocation((GLhandle)(size_t)s, i, attr);
  }
}

void kgmGLES::gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val)
{
  GLint link = gles.glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  switch(type) {
  case gcunitype_float1:
    gles.glUniform1fv(link, cnt, (float*)val);
    break;
  case gcunitype_float2:
    gles.glUniform2fv(link, cnt, (float*)val);
    break;
  case gcunitype_float3:
    gles.glUniform3fv(link, cnt, (float*)val);
    break;
  case gcunitype_float4:
    gles.glUniform4fv(link, cnt, (float*)val);
    break;
  case gcunitype_int1:
    gles.glUniform1iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int2:
    gles.glUniform2iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int3:
    gles.glUniform3iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int4:
    gles.glUniform4iv(link, cnt, (const int*)val);
    break;
  }

#ifdef DEBUG
  GLenum error = gles.glGetError();

  if (gles.glGetError() != GL_NO_ERROR)
  {
    fprintf(stderr, "Error glUniform: %d.\n", error);
  }
#endif
}

void kgmGLES::gcUniformMatrix(void* s, u32 type, u32 cnt, u32 trn, const char* par, void* val)
{
  GLint link = gles.glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  switch(type)
  {
  case gcunitype_mtx2:
    gles.glUniformMatrix2fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  case gcunitype_mtx3:
    gles.glUniformMatrix3fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  case gcunitype_mtx4:
    gles.glUniformMatrix4fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  }
}

void kgmGLES::gcUniformSampler(void* s, const char* par, void* val)
{
  GLint link = gles.glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  gles.glUniform1i(link, (u32)(size_t)val);
}

#ifdef DEBUG
void kgmGLES::gcGetUniform(void* s, const char* par, void* val)
{
  gles.glGetUniformfv((GLhandle)(size_t)s, (GLint)(size_t)par, (float*)val);
}
#endif


#endif
