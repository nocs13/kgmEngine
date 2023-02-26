// kgmOGL.cpp: implementation of the kgmOGL class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmOGL.h"
#include "kgmWindow.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmArray.h"

#define GL_DEPTH_STENCIL_EXT               GL_DEPTH_STENCIL_NV
#define GL_DEPTH_STENCIL_ATTACHMENT_EXT    0x821A
#define GLX_CONTEXT_PROFILE_MASK_ARB       0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB   0x00000001

#ifdef WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

GLint* g_compressed_format = null;
GLint  g_num_compressed_format = 0;

kgmIGC* kgmCreateOGLContext(kgmWindow* w)
{
  #ifdef OGL
  return new kgmOGL(w);
  #else
  return null;
  #endif
}

#ifdef OGL

#define GL_VER_MAJ 3
#define GL_VER_MIN 1

#ifdef WIN32
#else
  #include <GL/glext.h>
#endif

#define glActiveTexture           glActiveTextureARB
#define glClientActiveTexture     glClientActiveTextureARB
#define glGenBuffers              glGenBuffersARB
#define glDeleteBuffers           glDeleteBuffersARB
#define glBindBuffer              glBindBufferARB
#define glBufferData              glBufferDataARB
#define glBufferSubData           glBufferSubDataARB
#define glGetBufferSubData        glGetBufferSubDataARB
#define glMapBuffer               glMapBufferARB
#define glUnmapBuffer             glUnmapBufferARB
#define glGenFramebuffers         glGenFramebuffersEXT
#define glDeleteFramebuffers      glDeleteFramebuffersEXT
#define glBindFramebuffer         glBindFramebufferEXT
#define glFramebufferTexture2D    glFramebufferTexture2DEXT
#define glCheckFramebufferStatus  glCheckFramebufferStatusEXT
#define glGenRenderbuffers        glGenRenderbuffersEXT
#define glBindRenderbuffer        glBindRenderbufferEXT
#define glRenderbufferStorage     glRenderbufferStorageEXT
#define glFramebufferRenderbuffer glFramebufferRenderbufferEXT
#define glDeleteRenderbuffers     glDeleteRenderbuffersEXT
#define glCreateShaderObject      glCreateShaderObjectARB
#define glCreateProgramObject     glCreateProgramObjectARB
#define glDeleteObject            glDeleteObjectARB
#define glShaderSource            glShaderSourceARB
#define glCompileShader           glCompileShaderARB
#define glAttachObject            glAttachObjectARB
#define glDetachObject            glDetachObjectARB
#define glLinkProgram             glLinkProgramARB
#define glIsProgram               glIsProgramARB
#define glValidateProgram         glValidateProgramARB
#define glUseProgramObject        glUseProgramObjectARB
#define glGetObjectParameterfv    glGetObjectParameterfvARB
#define glGetObjectParameteriv    glGetObjectParameterivARB
#define glGetInfoLog              glGetInfoLogARB
#define glGetUniformLocation      glGetUniformLocationARB
#define glUniform1i               glUniform1iARB
#define glUniform1iv              glUniform1ivARB
#define glUniform2iv              glUniform2ivARB
#define glUniform3iv              glUniform3ivARB
#define glUniform4iv              glUniform4ivARB
#define glUniform1fv              glUniform1fvARB
#define glUniform2fv              glUniform2fvARB
#define glUniform3fv              glUniform3fvARB
#define glUniform4fv              glUniform4fvARB
#define glUniformMatrix2fv        glUniformMatrix2fvARB
#define glUniformMatrix3fv        glUniformMatrix3fvARB
#define glUniformMatrix4fv        glUniformMatrix4fvARB
#define glBindAttribLocation      glBindAttribLocationARB
#define glGetUniformfv            glGetUniformfvARB

#define glDisableVertexAttribArray glDisableVertexAttribArrayARB
#define glEnableVertexAttribArray  glEnableVertexAttribArrayARB
#define glVertexAttribPointer      glVertexAttribPointerARB
#define glGetAttribLocation        glGetAttribLocationARB

#define glBlendEquation    glBlendEquationEXT

#ifdef WIN32
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
extern  PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;
#endif

#ifdef LINUX
extern PFNGLXSWAPINTERVALSGIPROC glxSwapIntervalSGI;
#endif

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif

#define GL_DEBUG_OUTPUT_KHR               0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR   0x8242

struct {
  void (*glXSwapBuffers)(Display *dpy, GLXDrawable drawable);
  GLXContext (*glXCreateContext)(Display *dpy, XVisualInfo *vis, GLXContext share_list, Bool direct);
  GLXContext (*glXCreateContextAttribsARB) (Display *, GLXFBConfig, GLXContext, Bool, const int *);
  Bool (*glXMakeCurrent)(Display *, GLXDrawable, GLXContext);
  Bool (*glXIsDirect)(Display *dpy, GLXContext ctx);
  Bool (*glXQueryVersion)(Display *dpy, int *major, int *minor);
  void (*glXDestroyContext)(Display *dpy, GLXContext ctx);
  void* (*glXGetProcAddress)(const s8*);
} glx;


struct {
  const GLubyte* ( *glGetString)(GLenum name);
  void (* glEnable)(GLenum name);
  void (* glDisable)(GLenum name);
  GLenum (* glGetError)(void);
  void (* glGetIntegerv) (GLenum pname, GLint *params);
  void (* glDepthFunc)(GLenum func);
  void (* glCullFace) (GLenum mode);
  void (* glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
  void (* glClearDepth) (GLclampd depth);
  void (* glClearStencil)(GLint s);
  void (* glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  void (* glClear)(GLbitfield mask);
  void (* glFlush)(void);
  void (* glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
  void (* glBlendFunc)(GLenum sfactor, GLenum dfactor);
  void (* glDepthMask)(GLboolean flag);
  void (* glBindTexture)(GLenum target, GLu32 texture);;
  void (* glGenTextures)(GLsizei n, GLu32 *textures);
  void (* glTexParameteri)(GLenum target, GLenum pname, GLint param);
  void (* glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
  void (* glDeleteTextures)(GLsizei n, const GLu32 *textures);
  void (* glDrawBuffer) (GLenum mode);
  void (* glClipPlane) (GLenum plane, const GLdouble *equation);
  void (* glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
  void (* glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
  void (* glDisableClientState) (GLenum array);
  void (* glEnableClientState) (GLenum array);
  void (* glTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
//DEBUG
  //PFNGLDEBUGMESSAGECALLBACKKHRPROC glDebugMessageCallbackKHR;
//FRAME BUFFER EXTENTIONS
  PFNGLGENFRAMEBUFFERSEXTPROC          glGenFramebuffersEXT;
  PFNGLDELETEFRAMEBUFFERSEXTPROC       glDeleteFramebuffersEXT;
  PFNGLBINDFRAMEBUFFEREXTPROC          glBindFramebufferEXT;
  PFNGLFRAMEBUFFERTEXTURE2DEXTPROC     glFramebufferTexture2DEXT;
  PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC   glCheckFramebufferStatusEXT;
  PFNGLGENRENDERBUFFERSEXTPROC         glGenRenderbuffersEXT;
  PFNGLBINDRENDERBUFFEREXTPROC         glBindRenderbufferEXT;
  PFNGLRENDERBUFFERSTORAGEEXTPROC      glRenderbufferStorageEXT;
  PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC  glFramebufferRenderbufferEXT;
  PFNGLDELETERENDERBUFFERSEXTPROC      glDeleteRenderbuffersEXT;
  PFNGLBLENDEQUATIONEXTPROC            glBlendEquationEXT;
  PFNGLPOLYGONOFFSETEXTPROC            glPolygonOffsetEXT;
  PFNGLDRAWARRAYSEXTPROC               glDrawArraysEXT;
//TEXTURE EXTENTIONS
  PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
  PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB;
//VERTEX BUFFER EXTENTIONS
  PFNGLGENBUFFERSARBPROC			  glGenBuffersARB;
  PFNGLDELETEBUFFERSARBPROC		glDeleteBuffersARB;
  PFNGLBINDBUFFERARBPROC			  glBindBufferARB;
  PFNGLBUFFERDATAARBPROC			  glBufferDataARB;
  PFNGLBUFFERSUBDATAARBPROC		glBufferSubDataARB;
  PFNGLGETBUFFERSUBDATAARBPROC	glGetBufferSubDataARB;
  PFNGLMAPBUFFERARBPROC			  glMapBufferARB;
  PFNGLUNMAPBUFFERARBPROC			glUnmapBufferARB;
//GLSL SHADER EXTENTIONS
  PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB;
  PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB;
  PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB;
  PFNGLSHADERSOURCEARBPROC         glShaderSourceARB;
  PFNGLCOMPILESHADERARBPROC        glCompileShaderARB;
  PFNGLATTACHOBJECTARBPROC         glAttachObjectARB;
  PFNGLDETACHOBJECTARBPROC         glDetachObjectARB;
  PFNGLLINKPROGRAMARBPROC          glLinkProgramARB;
  PFNGLISPROGRAMARBPROC            glIsProgramARB;
  PFNGLVALIDATEPROGRAMARBPROC      glValidateProgramARB;
  PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB;
  PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
  PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
  PFNGLGETINFOLOGARBPROC           glGetInfoLogARB;
  PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB;
  PFNGLUNIFORM1IARBPROC            glUniform1iARB;
  PFNGLUNIFORM1IVARBPROC           glUniform1ivARB;
  PFNGLUNIFORM2IVARBPROC           glUniform2ivARB;
  PFNGLUNIFORM3IVARBPROC           glUniform3ivARB;
  PFNGLUNIFORM4IVARBPROC           glUniform4ivARB;
  PFNGLUNIFORM1FVARBPROC           glUniform1fvARB;
  PFNGLUNIFORM2FVARBPROC           glUniform2fvARB;
  PFNGLUNIFORM3FVARBPROC           glUniform3fvARB;
  PFNGLUNIFORM4FVARBPROC           glUniform4fvARB;
  PFNGLUNIFORMMATRIX2FVARBPROC     glUniformMatrix2fvARB;
  PFNGLUNIFORMMATRIX3FVARBPROC     glUniformMatrix3fvARB;
  PFNGLUNIFORMMATRIX4FVARBPROC     glUniformMatrix4fvARB;
  PFNGLBINDATTRIBLOCATIONARBPROC   glBindAttribLocationARB;
  PFNGLGETUNIFORMFVARBPROC         glGetUniformfvARB;
  PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
  PFNGLENABLEVERTEXATTRIBARRAYARBPROC  glEnableVertexAttribArrayARB;
  PFNGLVERTEXATTRIBPOINTERARBPROC      glVertexAttribPointerARB;
  PFNGLGETATTRIBLOCATIONARBPROC        glGetAttribLocationARB;
} ogl;

static void __onDebugMessage(GLenum source,GLenum type,GLint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam)
{
  kgm_log() << "OGL Debug log: " << message << ".\n";
}

kgmOGL::kgmOGL(kgmWindow *wnd)
{
  if(!wnd)
    return;

  this->m_wnd = wnd;

#ifdef DEBUG
  kgm_log() << "init OGL.\n";
#endif

#ifdef WIN32

  PIXELFORMATDESCRIPTOR pfd = {0};
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                PFD_SUPPORT_OPENGL |
                PFD_DOUBLEBUFFER;// |
  //				  PFD_GENERIC_ACCELERATED |
  //				   PFD_SWAP_LAYER_BUFFERS;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.cStencilBits = 8;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.iLayerType = PFD_MAIN_PLANE;

  m_hdc = GetDC(wnd->m_wnd);

  if(!m_hdc)
    return;

  int pf = ChoosePixelFormat(m_hdc,&pfd);

  if(!pf)
    return;

  if(!SetPixelFormat(m_hdc,pf,&pfd))
    return;

  m_hrc = wglCreateContext(m_hdc);

  if(!m_hrc)
    return;

  if(!wglMakeCurrent(m_hdc,m_hrc))
    return;

  SendMessage(wnd->m_wnd, WM_ACTIVATE, 0, 0);
  SendMessage(wnd->m_wnd, WM_PAINT, 0, 0);

#elif defined(DARWIN)

#else

  int rx, ry, rw, rh;

  wnd->getRect(rx, ry, rw, rh);

  if (!m_glx.open("libGLX.so"))
  {
    m_error = 1;

    return;
  }

  glx.glXGetProcAddress = (void* (*)(const s8*)) m_glx.get("glXGetProcAddress");

  if (glx.glXGetProcAddress == null)
  {
    m_error = 1;

    return;
  }

  #define GLX_FN(func)  glx.func = (typeof( glx.func )) glx.glXGetProcAddress(#func)

  GLX_FN(glXSwapBuffers);
  GLX_FN(glXCreateContext);
  GLX_FN(glXMakeCurrent);
  GLX_FN(glXIsDirect);
  GLX_FN(glXDestroyContext);
  GLX_FN(glXCreateContextAttribsARB);
  GLX_FN(glXQueryVersion);

  #define GL_FN(func)  ogl.func = (typeof( ogl.func )) glx.glXGetProcAddress(#func)

  GL_FN(glGetString);
  GL_FN(glEnable);
  GL_FN(glDisable);
  GL_FN(glGetError);
  GL_FN(glGetIntegerv);
  GL_FN(glDepthFunc);
  GL_FN(glCullFace);
  GL_FN(glColorMask);
  GL_FN(glClearDepth);
  GL_FN(glClearStencil);
  GL_FN(glClearColor);
  GL_FN(glClear);
  GL_FN(glFlush);
  GL_FN(glViewport);
  GL_FN(glBlendFunc);
  GL_FN(glDepthMask);
  GL_FN(glBindTexture);
  GL_FN(glGenTextures);
  GL_FN(glTexParameteri);
  GL_FN(glTexImage2D);
  GL_FN(glDeleteTextures);
  GL_FN(glClipPlane);
  GL_FN(glDrawElements);
  GL_FN(glScissor);

  GL_FN(glActiveTextureARB);
  GL_FN(glClientActiveTextureARB);
  GL_FN(glGenBuffersARB);
  GL_FN(glDeleteBuffersARB);
  GL_FN(glBindBufferARB);
  GL_FN(glBufferDataARB);
  GL_FN(glBufferSubDataARB);
  GL_FN(glGetBufferSubDataARB);
  GL_FN(glMapBufferARB);
  GL_FN(glUnmapBufferARB);
  GL_FN(glCreateShaderObjectARB);
  GL_FN(glCreateProgramObjectARB);
  GL_FN(glDeleteObjectARB);
  GL_FN(glShaderSourceARB);
  GL_FN(glCompileShaderARB);
  GL_FN(glAttachObjectARB);
  GL_FN(glDetachObjectARB);
  GL_FN(glLinkProgramARB);
  GL_FN(glIsProgramARB);
  GL_FN(glValidateProgramARB);
  GL_FN(glUseProgramObjectARB);
  GL_FN(glGetObjectParameterfvARB);
  GL_FN(glGetObjectParameterivARB);
  GL_FN(glGetInfoLogARB);
  GL_FN(glGetUniformLocationARB);
  GL_FN(glUniform1iARB);
  GL_FN(glUniform1ivARB);
  GL_FN(glUniform2ivARB);
  GL_FN(glUniform3ivARB);
  GL_FN(glUniform4ivARB);
  GL_FN(glUniform1fvARB);
  GL_FN(glUniform2fvARB);
  GL_FN(glUniform3fvARB);
  GL_FN(glUniform4fvARB);
  GL_FN(glUniformMatrix2fvARB);
  GL_FN(glUniformMatrix3fvARB);
  GL_FN(glUniformMatrix4fvARB);
  GL_FN(glBindAttribLocationARB);
  GL_FN(glGetUniformfvARB);
  GL_FN(glDisableVertexAttribArrayARB);
  GL_FN(glEnableVertexAttribArrayARB);
  GL_FN(glVertexAttribPointerARB);
  GL_FN(glGetAttribLocationARB);

  GL_FN(glGenFramebuffersEXT);
  GL_FN(glDeleteFramebuffersEXT);
  GL_FN(glBindFramebufferEXT);
  GL_FN(glFramebufferTexture2DEXT);
  GL_FN(glCheckFramebufferStatusEXT);
  GL_FN(glGenRenderbuffersEXT);
  GL_FN(glBindRenderbufferEXT);
  GL_FN(glRenderbufferStorageEXT);
  GL_FN(glFramebufferRenderbufferEXT);
  GL_FN(glDeleteRenderbuffersEXT);
  GL_FN(glBlendEquationEXT);
  GL_FN(glPolygonOffsetEXT);
  GL_FN(glDrawArraysEXT);
  GL_FN(glCheckFramebufferStatusEXT);

  if (!m_glctx)
  {
    if (wnd->m_visual)
    {
      m_glctx = glx.glXCreateContext(wnd->m_dpy, wnd->m_visual, 0, GL_TRUE);
    }

    if (m_glctx && GL_VER_MAJ > 2)
    {
      int glx_major = 0, glx_minor = 0;

      glx.glXQueryVersion(wnd -> m_dpy, &glx_major, &glx_minor);

      kgm_log() << "GLX version: " << glx_major << "." << glx_minor << ".\n";

      int attrs[] = { GLX_CONTEXT_MAJOR_VERSION_ARB,  GL_VER_MAJ,
                      GLX_CONTEXT_MINOR_VERSION_ARB,  GL_VER_MIN,
                      0 };

      /* Create a GL 3.x context */
      GLXFBConfig *framebuffer_config = NULL;
      int fbcount = 0;
      GLXFBConfig *(*glXChooseFBConfig) (Display * disp,
                                         int screen,
                                         const int *attrib_list,
                                         int *nelements);

      glXChooseFBConfig = (typeof glXChooseFBConfig) glx.glXGetProcAddress((s8 *) "glXChooseFBConfig");

      if (!glXChooseFBConfig
          || !(framebuffer_config =
               glXChooseFBConfig(wnd->m_dpy, wnd->m_screen, NULL, &fbcount)))
      {

        kgm_log() << "No good framebuffers found. GL 3.x disabled.\n";
      }
      else
      {
        GLXContext ctx = glx.glXCreateContextAttribsARB(m_wnd->m_dpy, framebuffer_config[0],  NULL, True, attrs);

        if (ctx != NULL)
        {
          glx.glXDestroyContext(m_wnd->m_dpy, m_glctx);

          m_glctx = ctx;
        }
      }
    }
  }

  if (!m_glctx)
  {
    kgm_log() << "Error: Cannot create ogl context.\n";

    m_error = 1;

    return;
  }

  if(!glx.glXMakeCurrent(wnd->m_dpy, wnd->m_wnd, m_glctx))
  {
    kgm_log() << "Error: Cannot activate ogl context.\n";

    m_error = 1;

    return;
  }

  if(glx.glXIsDirect(wnd->m_dpy, m_glctx))
    kgm_log() << "Direct Rendering!\n";
  else
    kgm_log() << "Not direct Rendering!\n";

#endif

#ifdef DEBUG
  kgm_log() << "OpenGL Version: " << (char*) ogl.glGetString(GL_VERSION) << "\n";
  kgm_log() << "GLSL   Version: " << (char*) ogl.glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
  kgm_log() << "OpenGL Vendor: " << (char*) ogl.glGetString(GL_VENDOR) << "\n";
  kgm_log() << "OpenGL Render: " << (char*) ogl.glGetString(GL_RENDERER) << "\n";
#endif


  //glInitExt();

  ogl.glEnable(GL_TEXTURE_CUBE_MAP);
  ogl.glEnable(GL_TEXTURE_2D);
  ogl.glEnable(GL_DEPTH_TEST);
  ogl.glDepthFunc(GL_LEQUAL);
  ogl.glEnable(GL_CULL_FACE);
  ogl.glCullFace(GL_BACK);

  const GLubyte* ext = ogl.glGetString(GL_EXTENSIONS);

#ifdef DEBUG
  kgm_log() << "OGL Extentions: " << (s8*) ext << ".\n";
#endif

  if (!ext)
  {
    return;
  }

  if(strstr((char*)ext, "GL_ARB_shader_objects"))
  {
    m_error = 2;
  }

  if(strstr((char*)ext, "GL_ARB_framebuffer_object"))
  {
    m_error = 3;
  }


#ifdef GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB
  ogl.glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &g_num_compressed_format);

  g_compressed_format = (GLint*)kgm_alloc(g_num_compressed_format * sizeof(GLint));

  ogl.glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, g_compressed_format);

  for(int i = 0; i < g_num_compressed_format; i++)
  {
    int k = g_compressed_format[i];
    int a = 0;
  }
#endif

#ifdef WIN32
  //if(wglSwapIntervalEXT) wglSwapIntervalEXT(0);
#endif

#ifdef LINUX
  //if(glxSwapIntervalSGI) glxSwapIntervalSGI(0);
#endif

  GLint max_image_units = 0;

  ogl.glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_image_units);

  //init local values
  ogl.glEnable(GL_TEXTURE_2D);

  ogl.glPolygonOffsetEXT(1.0f, 1.0f);

  m_renderbuffer = 0;

  m_min_filter = GL_LINEAR;
  m_mag_filter = GL_LINEAR;
}

kgmOGL::~kgmOGL()
{
#ifdef GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB
  if(g_compressed_format)
    kgm_free(g_compressed_format);
#endif

#ifdef WIN32

  wglDeleteContext(m_hrc);
  wglMakeCurrent(m_hdc, 0);
  ReleaseDC(m_wnd->m_wnd,m_hdc);

#elif defined(DARWIN)
#else

  if (m_glctx)
  {
    if(!glx.glXMakeCurrent(m_wnd->m_dpy, None, NULL))
      kgmLog::log("Could not release drawing context.\n");

    glx.glXDestroyContext(m_wnd->m_dpy, m_glctx);

    m_glctx = NULL;
  }

#endif

#ifdef DEBUG
  kgm_log() << "kgmOGL::~kgmOGL.\n";
#endif
}

void kgmOGL::gcSet(u32 param, void* value)
{
  u32 tmp = 0;
  switch(param)
  {
  case gcpar_blending:
    if(value)
      ogl.glEnable(GL_BLEND);
    else
      ogl.glDisable(GL_BLEND);
    break;
  case gcpar_culling:
    if(value)
      ogl.glEnable(GL_CULL_FACE);
    else
      ogl.glDisable(GL_CULL_FACE);
    break;
  case gcpar_textures:
    if(value)
      ogl.glEnable(GL_TEXTURE_2D);
    else
      ogl.glDisable(GL_TEXTURE_2D);
    break;
  case gcpar_depth:
    if(value)
      ogl.glEnable(GL_DEPTH_TEST);
    else
      ogl.glDisable(GL_DEPTH_TEST);
    break;
#ifdef GL_ALPHA_TEST
  case gcpar_alpha:
    if(value)
      ogl.glEnable(GL_ALPHA_TEST);
    else
      ogl.glDisable(GL_ALPHA_TEST);
    break;
#endif
  case gcpar_stencil:
    if(value)
      ogl.glEnable(GL_STENCIL_TEST);
    else
      ogl.glDisable(GL_STENCIL_TEST);
    break;
  case gcpar_colormask:
  {
    char* c = (char*)&value;
    GLboolean r = (c[0])?(GL_TRUE):(GL_FALSE),
        g = (c[1])?(GL_TRUE):(GL_FALSE),
        b = (c[2])?(GL_TRUE):(GL_FALSE),
        a = (c[3])?(GL_TRUE):(GL_FALSE);
    ogl.glColorMask(r, g, b, a);
  }
    break;
  case gcpar_clipplane:
  {
    if (value)
    {
      ClipPlane* cp = (ClipPlane*) value;

      if (cp->on)
      {
        double pl[] = { cp->plane[0], cp->plane[1], cp->plane[2], cp->plane[3] };
        ogl.glEnable(GL_CLIP_PLANE0 + cp->id);
        ogl.glClipPlane(GL_CLIP_PLANE0 + cp->id, pl);
      }
      else
      {
        ogl.glDisable(GL_CLIP_PLANE0 + cp->id);
      }
    }
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

void kgmOGL::gcGet(u32 param, void* value)
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

u32 kgmOGL::gcError()
{
  return m_error;
}

void kgmOGL::gcClear(u32 flag, u32 col, float depth, u32 sten)
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

    ogl.glClearColor(r, g, b, a);
  }

  if(flag & gcflag_depth)
  {
    cl |= GL_DEPTH_BUFFER_BIT;
    ogl.glClearDepth(depth);
  }

  if(flag & gcflag_stencil)
  {
    cl |= GL_STENCIL_BUFFER_BIT;
    ogl.glClearStencil(sten);
  }

  ogl.glClear(cl);
}

void kgmOGL::gcBegin()
{
}

void kgmOGL::gcEnd()
{
}

void kgmOGL::gcRender()
{
#ifdef WIN32

  SwapBuffers(m_hdc);

#elif defined(DARWIN)
#else

  glx.glXSwapBuffers(m_wnd->m_dpy, m_wnd->m_wnd);

#endif
}

void kgmOGL::gcResize(u32 width, u32 height)
{

}

void kgmOGL::gcSetViewport(int x, int y, int w, int h, float n, float f)
{
  m_viewport[0] = x;
  m_viewport[1] = y;
  m_viewport[2] = w;
  m_viewport[3] = h;
  ogl.glViewport(x, y, w, h);
}


//BLEND
void  kgmOGL::gcBlend(bool en, u32 eq, u32 fnsrc, u32 fndst)
{
  if(!en)
  {
    ogl.glDisable(GL_BLEND);

    return;
  }

  switch(eq)
  {
  case gcblend_eqadd:
    ogl.glBlendEquation(GL_FUNC_ADD_EXT);
    break;
  case gcblend_eqsub:
    ogl.glBlendEquation(GL_FUNC_SUBTRACT_EXT);
    break;
  case gcblend_eqmin:
    ogl.glBlendEquation(GL_MIN);
    break;
  case gcblend_eqmax:
    ogl.glBlendEquation(GL_MAX);
    break;
  case gcblend_eqrsub:
    ogl.glBlendEquation(GL_FUNC_REVERSE_SUBTRACT_EXT);
    break;
  default:
    ogl.glBlendEquation(GL_FUNC_ADD);
  }

  ogl.glBlendFunc(gl_enum(fnsrc), gl_enum(fndst));
  ogl.glEnable(GL_BLEND);
}

//CULL
void kgmOGL::gcCull(u32 mode)
{
  if(!mode)
  {
    ogl.glDisable(GL_CULL_FACE);

    return;
  }

  switch(mode)
  {
  case gccull_back:
    ogl.glCullFace(GL_BACK);
    break;
  case gccull_front:
    ogl.glCullFace(GL_FRONT);
    break;
  }

  ogl.glEnable(GL_CULL_FACE);
}

//DEPTH
void kgmOGL::gcDepth(bool depth, bool mask, u32 mode)
{
  if(depth)
    ogl.glEnable(GL_DEPTH_TEST);
  else
    ogl.glDisable(GL_DEPTH_TEST);

  if(mask)
    ogl.glDepthMask(GL_TRUE);
  else
    ogl.glDepthMask(GL_FALSE);

  ogl.glDepthFunc(gl_enum(mode));
}

//TEXTURE
gchandle kgmOGL::gcGenTexture(void *pd, u32 w, u32 h, u32 fmt, u32 type)
{
  GLu32  tex     = 0;
  GLenum pic_fmt = 0;
  GLu32  int_fmt = 0;
  u32    bpc     = 1;

#ifdef DEBUG
  kgm_log() << "gcGenTexture " << (s32)w << " " << (s32)h << " " << (s32)fmt << "\n";
#endif

  switch(fmt)
  {
  case gctex_fmt8:
    pic_fmt = GL_RGB;
    int_fmt = GL_UNSIGNED_BYTE_3_3_2;
    break;
  case gctex_fmt16:
    pic_fmt = GL_RGB;
    int_fmt = GL_UNSIGNED_SHORT_5_6_5;
    bpc = 2;
    break;
  case gctex_fmt24:
    pic_fmt = GL_RGB;
    //pic_fmt = GL_BGR;
    int_fmt = GL_RGB8;
    bpc = 3;
    break;
  case gctex_fmt32:
    pic_fmt = GL_RGBA;
    //pic_fmt = GL_BGRA;
    int_fmt = GL_RGBA8;
    bpc = 4;
    break;
  case gctex_fmtdepth:
    pic_fmt = GL_DEPTH_COMPONENT;
    int_fmt = GL_DEPTH_COMPONENT;
    break;
  case gctex_fmtdepcil:
    pic_fmt = GL_DEPTH_STENCIL_EXT;
    int_fmt = GL_DEPTH_STENCIL_EXT;
    break;
  }

  u8* cnt = null;

  switch(type)
  {
  case gctype_tex2d:
    ogl.glGenTextures(1, &tex);
    ogl.glBindTexture(GL_TEXTURE_2D, tex);
    break;
  case gctype_texcube:
    ogl.glGenTextures(1, &tex);
    ogl.glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
    break;
  }

  switch(type)
  {
  case gctype_tex2d:
    if(fmt == gctex_fmtdepth)
    {
      ogl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      ogl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      ogl.glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    }
    else
    {
      ogl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
      ogl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);

      ogl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      ogl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    ogl.glTexImage2D(GL_TEXTURE_2D, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, pd);
#ifdef DEBUG
    if (ogl.glGetError() != GL_NO_ERROR)
      kgm_log() << "Error: Cannot create 2D texture. eid=" << (s32) ogl.glGetError() << ".\n";
#endif
    break;
  case gctype_texcube:
    cnt = (u8*) kgm_alloc(bpc * w * h);
    memset(cnt, 0xff, bpc * w * h);

    ogl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ogl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ogl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ogl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ogl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    ogl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    ogl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    ogl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    ogl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    ogl.glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    ogl.glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    kgm_free(cnt);
    break;
  }

  switch(type)
  {
  case gctype_texcube:
    ogl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    break;
  default:
    ogl.glBindTexture(GL_TEXTURE_2D, 0);
  }

#ifdef DEBUG
  kgm_log() << "Gen texture: " << (s32) tex << "\n";
#endif

  return (gchandle) ((size_t) tex);
}

void kgmOGL::gcFreeTexture(gchandle t)
{
  if(!t)
    return;

  GLu32 tex = (GLu32) (size_t) t;

  if(t)
    ogl.glDeleteTextures(1, &tex);


#ifdef DEBUG
  kgm_log() << "Free texture: " << (s32) tex << "\n";
#endif
}

void kgmOGL::gcSetTexture(u32 stage, gchandle t)
{
  if(!t)
  {
    ogl.glActiveTexture(GL_TEXTURE0 + stage);
    ogl.glBindTexture(GL_TEXTURE_2D, 0);

    if (ogl.glGetError() == GL_INVALID_OPERATION)
    {
      ogl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

      if (ogl.glGetError() != GL_NO_ERROR)
      {
        int k = 0;
      }
    }

    return;
  }

  ogl.glActiveTexture(GL_TEXTURE0 + stage);
  ogl.glBindTexture(GL_TEXTURE_2D, (GLu32) (size_t) t);

  if (ogl.glGetError() == GL_INVALID_OPERATION)
  {
    ogl.glBindTexture(GL_TEXTURE_CUBE_MAP, (GLu32) (size_t) t);

    if (ogl.glGetError() != GL_NO_ERROR)
    {
      int k = 0;
    }
  }
}

// TARGET
gchandle kgmOGL::gcGenTarget(u32 w, u32 h, bool d, bool s)
{
  GLu32 buffer = 0;
  ogl.glGenFramebuffers(1, &buffer);
  ogl.glBindFramebuffer(GL_FRAMEBUFFER_EXT, buffer);
  ogl.glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

  /*
  GLu32 texture = 0;

  switch (type)
  {
  case gctype_texcube:
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    break;
  case gctype_texdepth:
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    break;
  case gctype_tex2d:
  default:
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  };

  glBindTexture(GL_TEXTURE_2D, 0);
  */

  GLu32 depth = 0;

  if (d)
  {
    ogl.glGenRenderbuffers(1, &depth);
    ogl.glBindRenderbuffer(GL_RENDERBUFFER_EXT, depth);

    if (s)
    {
      ogl.glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL_EXT, w, h);
      ogl.glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
    }
    else
    {
      ogl.glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
      ogl.glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
    }
  }

  GLu32 stencil = 0;

  /*
  switch (type)
  {
  case gctype_texdepth:
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, texture, 0);
    break;
  case gctype_tex2d:
  default:
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
  };
  */

#ifdef DEBUG
  GLenum status;
  status = ogl.glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

  if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
  {
    kgm_log() << "Error: cannot use framebuffer object!\n";
    return null;
  }
#endif

  ogl.glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  ogl.glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);

  RenderBuffer* rb = (RenderBuffer*) kgm_alloc(sizeof(RenderBuffer));

  rb->frame   = buffer;
  rb->color   = 0;
  rb->depth   = depth;
  rb->stencil = stencil;
  rb->width   = w;
  rb->height  = h;
  rb->cmside  = 0;

  return (gchandle) rb;
}

bool kgmOGL::gcTexTarget(gchandle tar, gchandle tex, u32 type)
{
  RenderBuffer* rb = static_cast<RenderBuffer*>(tar);

  if(!rb)
    return false;

  ogl.glBindFramebuffer(GL_FRAMEBUFFER_EXT, rb->frame);
  ogl.glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  ogl.glBindRenderbuffer(GL_RENDERBUFFER_EXT, rb->depth);

  switch (type)
  {
  case gctype_texdepth:
    ogl.glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D,  (GLint) (size_t) tex, 0);
    break;
  case gctype_tex2d:
    ogl.glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, (GLint) (size_t) tex, 0);
    break;
  case gctype_texcube:
    ogl.glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_cubemapside, (GLint) (size_t) tex, 0);

    ++rb->cmside;

    if (rb->cmside > 5)
      rb->cmside = 0;
    break;
  default:
    ogl.glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, (GLint)            0, 0);
  };

  rb->type = type;
  rb->color = (GLint) (size_t) tex;

#ifdef DEBUG
  GLenum status = ogl.glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

  if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
    kgm_log() << "Error: Attaching texture to framebuffer not completed.\n";
    kgm_log() << "Error: " << (s32)status << ".\n";
  }
#endif

  ogl.glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  ogl.glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);

  return true;
}

void kgmOGL::gcFreeTarget(gchandle t)
{
  RenderBuffer* rb = static_cast<RenderBuffer*>(t);

  if(!rb)
    return;

  if (rb->depth)
    ogl.glDeleteRenderbuffers(1, &rb->depth);

  if (rb->frame)
    ogl.glDeleteFramebuffers(1, &rb->frame);

  kgm_free(rb);
}

void kgmOGL::gcSetTarget(gchandle t)
{
  RenderBuffer* rb = (RenderBuffer*) t;

  if(!rb || !rb->frame)
  {
    ogl.glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
    ogl.glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);

    m_renderbuffer = null;

    return;
  }

  ogl.glBindFramebuffer(GL_FRAMEBUFFER_EXT, rb->frame);
  //glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  //glBindRenderbuffer(GL_RENDERBUFFER_EXT, rb->depth);

  m_renderbuffer = rb;

#ifdef DEBUG
  GLenum err = ogl.glGetError();

  if (err != GL_NO_ERROR) {
    kgm_log() << "Error: Cannot set framebuffer.\n";
    kgm_log() << "Error: Eid is " << (s32) err << ".\n";
  }
#endif
}

//STENCIL
void kgmOGL::gcStencil(bool en, u32 func, u32 mask, u32 ref,
               u32 fail, u32 zfail, u32 zpass)
{

}

//SCISSOR
void kgmOGL::gcScissor(bool on, int x, int y, int w, int h)
{
  ogl.glScissor(x, y, w, h);

  if (on)
    ogl.glEnable(GL_SCISSOR_TEST);
  else
    ogl.glDisable(GL_SCISSOR_TEST);
}

//Drawing
void kgmOGL::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt,
                    u32 i_size, u32 i_cnt, void *i_pnt)
{
  if(!v_pnt)
    return;

  unsigned char *pM = (unsigned char*)v_pnt;
  unsigned int  uv_size = sizeof(float) * 2;

  int ah = 0;

  ogl.glBindBuffer(GL_ARRAY_BUFFER, 0);
  ogl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(v_fmt & gcv_xyz)
  {
    ah = ogl.glGetAttribLocation(m_shader, "a_Vertex");

    if(ah != -1)
    {
      ogl.glVertexAttribPointer(ah, 3, GL_FLOAT, GL_FALSE, v_size, pM);
      ogl.glEnableVertexAttribArray(ah);
    }

    pM += (sizeof(float) * 3);
  }

  if(v_fmt & gcv_nor)
  {
    ah = ogl.glGetAttribLocation(m_shader, "a_Normal");

    if(ah != -1)
    {
      ogl.glEnableVertexAttribArray(ah);
      ogl.glVertexAttribPointer(ah, 3, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (sizeof(float)*3);
  }

  if(v_fmt & gcv_col)
  {
    ah = ogl.glGetAttribLocation(m_shader, "a_Color");

    if(ah != -1)
    {
      ogl.glEnableVertexAttribArray(ah);
      ogl.glVertexAttribPointer(ah, 4, GL_FLOAT, GL_TRUE, v_size, pM);
    }

    pM += (sizeof(float) * 4);
  }

  if(v_fmt & gcv_uv0)
  {
    ah = ogl.glGetAttribLocation(m_shader, "a_UV");

    if(ah != -1)
    {
      ogl.glEnableVertexAttribArray(ah);
      ogl.glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_uv1)
  {
    ah = ogl.glGetAttribLocation(m_shader, "a_UV2");
    if(ah != -1)
    {
      ogl.glEnableVertexAttribArray(ah);
      ogl.glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_uv2)
  {
    ah = ogl.glGetAttribLocation(m_shader, "a_UV3");

    if(ah != -1)
    {
      ogl.glEnableVertexAttribArray(ah);
      ogl.glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_bn0)
  {
    ah = ogl.glGetAttribLocation(m_shader, "a_Weights");
    ogl.glEnableVertexAttribArray(ah);
    ogl.glVertexAttribPointer(ah, 4, GL_FLOAT, GL_FALSE, v_size, pM);
    pM += (4 * sizeof(float));
    ah = ogl.glGetAttribLocation(m_shader, "a_Indices");
    ogl.glEnableVertexAttribArray(ah);
    ogl.glVertexAttribPointer(ah, 4, GL_FLOAT, GL_FALSE, v_size, pM);
    pM += (4 * sizeof(float));
    /*u32 k = ((u32)pM - (u32)v_pnt);
    float* f1 = (float*)pM;
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_FLOAT, v_size, pM);
    pM += (4 * sizeof(float));
    k = ((u32)pM - (u32)v_pnt);
    float* f2 = (float*)pM;
    glClientActiveTexture(GL_TEXTURE2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_FLOAT, v_size, pM);
    pM += (4 * sizeof(float));*/
  }

  if(i_pnt && i_cnt)
  {
    switch(i_size)
    {
    case 4:
      ogl.glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_INT, i_pnt);
      //glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_INT, i_pnt);
      break;
    default:
      ogl.glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_SHORT,i_pnt);
      //glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_SHORT, i_pnt);
    }
  }
  else
  {
    ogl.glDrawArraysEXT(gl_enum(pmt), 0, v_cnt);
  }

  ogl.glDisableVertexAttribArray(0);
  ogl.glDisableVertexAttribArray(1);
  ogl.glDisableVertexAttribArray(2);
  ogl.glDisableVertexAttribArray(3);
  ogl.glDisableVertexAttribArray(4);
  ogl.glDisableVertexAttribArray(5);
}

//VERTEX & INDEX BUFFER
void* kgmOGL::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize)
{
  VBO* vbo = new VBO;
  memset(vbo, 0, sizeof(VBO));

  if(vdata && vsize)
  {
    ogl.glGenBuffers(1, &vbo->vb);
    ogl.glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
    ogl.glBufferData(GL_ARRAY_BUFFER, vsize, vdata, GL_STATIC_DRAW);
    vbo->vsize = vsize;
  }

  if(idata && isize)
  {
    ogl.glGenBuffers(1, &vbo->ib);
    ogl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);
    ogl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, idata, GL_STATIC_DRAW);
    vbo->isize = isize;
  }

  ogl.glBindBuffer(GL_ARRAY_BUFFER, 0);
  ogl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return (void*)vbo;
}

void  kgmOGL::gcFreeVertexBuffer(void* b)
{
  VBO* vbo = (VBO*)b;

  if(!vbo)
    return;

  if(vbo->vb)
    ogl.glDeleteBuffers(1, &vbo->vb);

  if(vbo->ib)
    ogl.glDeleteBuffers(1, &vbo->ib);

  delete vbo;
}

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

void  kgmOGL::gcDrawVertexBuffer(void* b, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt,
                                 u32 isize, u32 icnt, u32 ioff)
{
  VBO* vbo = (VBO*)b;
  size_t offset = 0;
  u32 uvt = 2;

  int al = 0;

  if(!vbo)
    return;

  //glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

  if(vbo->vb)
  {
    ogl.glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
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
    al = ogl.glGetAttribLocation(m_shader, "a_Vertex");

    if(al != -1)
    {
      ogl.glEnableVertexAttribArray(al);
      ogl.glVertexAttribPointer(al, 3, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 3;
    }
  }

  if(vfmt & gcv_nor)
  {
    al = ogl.glGetAttribLocation(m_shader, "a_Normal");

    if(al != -1)
    {
      ogl.glEnableVertexAttribArray(al);
      ogl.glVertexAttribPointer(al, 3, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 3;
    }
  }

  if(vfmt & gcv_col)
  {
    al = ogl.glGetAttribLocation(m_shader, "a_Color");

    if(al != -1)
    {
      ogl.glEnableVertexAttribArray(al);
      ogl.glVertexAttribPointer(al, 4, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 4;
    }
  }

  if(vfmt & gcv_uv0)
  {
    al = ogl.glGetAttribLocation(m_shader, "a_UV");

    if(al != -1)
    {
      ogl.glEnableVertexAttribArray(al);
      ogl.glVertexAttribPointer(al, 2, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * uvt;
    }
  }

  if(vfmt & gcv_uv1)
  {
    ogl.glClientActiveTexture(GL_TEXTURE1);
    ogl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ogl.glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv2)
  {
    ogl.glClientActiveTexture(GL_TEXTURE2);
    ogl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ogl.glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv3)
  {
    ogl.glClientActiveTexture(GL_TEXTURE3);
    ogl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ogl.glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv4)
  {
    ogl.glClientActiveTexture(GL_TEXTURE4);
    ogl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ogl.glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv5)
  {
    ogl.glClientActiveTexture(GL_TEXTURE5);
    ogl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ogl.glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv6)
  {
    ogl.glClientActiveTexture(GL_TEXTURE6);
    ogl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ogl.glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv7)
  {
    ogl.glClientActiveTexture(GL_TEXTURE7);
    ogl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ogl.glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vbo->ib && icnt)
  {
    //  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    ogl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);

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
      ogl.glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_INT, BUFFER_OFFSET(ioff));
      //glDrawRangeElements (gl_enum(pmt), 0, vcnt - 1, icnt, GL_UNSIGNED_INT, ((char*)0L + ioff));
      break;
    default:
      //glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);
      ogl.glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_SHORT, BUFFER_OFFSET(ioff));
      //glDrawRangeElements (gl_enum(pmt), ioff, ioff + icnt, icnt/3, GL_UNSIGNED_SHORT, (void*)0);
    }
  }
  else
  {
    ogl.glDrawArraysEXT(gl_enum(pmt), 0, vcnt);
  }

  ogl.glBindBuffer( GL_ARRAY_BUFFER, 0);
  ogl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  ogl.glDisableClientState(GL_VERTEX_ARRAY);
  ogl.glDisableClientState(GL_NORMAL_ARRAY);
  ogl.glDisableClientState(GL_COLOR_ARRAY);
  ogl.glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  //glPopClientAttrib();
}

// SHADERS
gchandle kgmOGL::gcGenShader(kgmArray<u8>& vsrc, kgmArray<u8>& fsrc)
{
  kgm_log() << "gcGenShader: Generating.\n";

  kgmString vstr;
  vstr.alloc((char *) vsrc.data(), vsrc.length());

  GLhandle prog = 0;
  GLhandle vshad = 0, fshad = 0;
  int stat[1] = {0};
  int size = 256;
  int gerr = 0;

#ifdef DEBUG
  char tbuf[512];
#endif

  kgm_log() << "gcGenShader: Creating program.\n";

  prog = ogl.glCreateProgramObject();

  gerr = ogl.glGetError();

  kgm_log() << "gcGenShader: Creating program status " << gerr << ".\n";

  if(GL_NO_ERROR != gerr)
  {
    int s = gerr;

#ifdef DEBUG
    kgm_log() << "gcGenShader: error is " << (s32) gerr << "\n";
#endif

    return null;
  }

  //GL_VERTEX_SHADER
  if(vsrc.data() && vsrc.length())
  {
    const char *ptr = (const char*) vsrc.data();

    size = strlen(ptr);
    vshad = ogl.glCreateShaderObject(GL_VERTEX_SHADER);
    ogl.glShaderSource(vshad, 1, (const GLcharARB**)&ptr, &size);
    ogl.glCompileShader(vshad);
    ogl.glGetObjectParameteriv(vshad, GL_OBJECT_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE)
    {
#ifdef DEBUG
      ogl.glGetInfoLog(vshad, sizeof(tbuf), &size, tbuf);
      kgm_log() << "gcGenShader: VShader> " << (char*)tbuf << " " << (s32)strlen(tbuf) << "\n";
#endif

      ogl.glDeleteObject(vshad);
      vshad = 0;
    }
    else
    {
      ogl.glAttachObject(prog, vshad);
    }
  }

  if(fsrc.data() && fsrc.length())
  {
    const char *ptr = (const char*) fsrc.data();

    size = strlen(ptr);
    fshad = ogl.glCreateShaderObject(GL_FRAGMENT_SHADER);
    ogl.glShaderSource(fshad, 1, (const GLcharARB**)&ptr, NULL);
    ogl.glCompileShader(fshad);
    ogl.glGetObjectParameteriv(fshad, GL_OBJECT_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE)
    {
#ifdef DEBUG
      ogl.glGetInfoLog(fshad, sizeof(tbuf), &size, tbuf);
      kgm_log() << "gcGenShader: FShader> " << (char*)tbuf << " " << (s32)strlen(tbuf) << "\n";
#endif

      ogl.glDeleteObject(vshad);
      fshad = 0;
    }
    else
    {
      ogl.glAttachObject(prog, fshad);
    }
  }

  //glBindAttribLocation((GLhandle)prog, 0, "g_Vertex");
  //glBindAttribLocation((GLhandle)prog, 1, "g_Normal");
  //glBindAttribLocation((GLhandle)prog, 2, "g_Color");
  //glBindAttribLocation((GLhandle)prog, 3, "g_Texcoord");

  ogl.glLinkProgram(prog);

  stat[0] = 0;

  ogl.glGetObjectParameteriv(prog, GL_LINK_STATUS, stat);

  if(stat[0] == GL_FALSE)
  {
#ifdef DEBUG
    ogl.glGetInfoLog(prog, sizeof(tbuf), &size, tbuf);
    kgm_log() << "gcGenShader: Link log> " << tbuf << "\n";
#endif

    ogl.glDeleteObject(vshad);
    ogl.glDeleteObject(fshad);
    ogl.glDeleteObject(prog);

    return null;
  }

  ogl.glValidateProgram(prog);

  stat[0] = 0;

  ogl.glGetObjectParameteriv(prog, GL_VALIDATE_STATUS, stat);

  if(stat[0] == GL_FALSE)
  {
#ifdef DEBUG
    ogl.glGetInfoLog(prog, sizeof(tbuf), &size, tbuf);
    kgm_log() << "gcGenShader: Validate log> " << tbuf << "\n";
#endif

    ogl.glDeleteObject(vshad);
    ogl.glDeleteObject(fshad);
    ogl.glDeleteObject(prog);

    return null;
  }

  kgm_log() << "gcGenShader: program id " << (s32) prog << ".\n";

  return (gchandle) (size_t) prog;
}

void kgmOGL::gcFreeShader(gchandle s)
{
  if(ogl.glIsProgram((GLint) (size_t) s) != GL_TRUE)
  {
#ifdef DEBUG
    kgm_log() << "gcFreeShader: Invalid shader program " << (s32) (size_t) s << "\n";
#endif
    return;
  }

#ifdef DEBUG
  kgm_log() << "gcFreeShader: shader program id " << (s32) (size_t) s << ".\n";
#endif

  ogl.glDeleteObject((GLint) (size_t) s);
}

void kgmOGL::gcSetShader(gchandle s)
{
  if (s == null)
  {
    ogl.glUseProgramObject(0);

    return;
  }

  GLint prog = 0;

  ogl.glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

  if (prog == (GLint) (size_t) s)
  {
    return;
  }

  ogl.glUseProgramObject((GLhandle)(size_t)s);

  m_shader = (GLhandle)(size_t)s;
}

void  kgmOGL::gcBindAttribute(void* s, int i, const char* attr)
{
  if(s)
  {
    ogl.glBindAttribLocation((GLhandle)(size_t)s, i, attr);
  }
}

void kgmOGL::gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val)
{
  GLint link = ogl.glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  switch(type){
  case gcunitype_float1:
    ogl.glUniform1fv(link, cnt, (float*)val);
    break;
  case gcunitype_float2:
    ogl.glUniform2fv(link, cnt, (float*)val);
    break;
  case gcunitype_float3:
    ogl.glUniform3fv(link, cnt, (float*)val);
    break;
  case gcunitype_float4:
    ogl.glUniform4fv(link, cnt, (float*)val);
    break;
  case gcunitype_int1:
    ogl.glUniform1iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int2:
    ogl.glUniform2iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int3:
    ogl.glUniform3iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int4:
    ogl.glUniform4iv(link, cnt, (const int*)val);
    break;
  }

#ifdef DEBUG
  GLenum error = ogl.glGetError();

  if (ogl.glGetError() != GL_NO_ERROR)
  {
    fprintf(stderr, "Error glUniform: %d.\n", error);
  }
#endif
}

void kgmOGL::gcUniformMatrix(void* s, u32 type, u32 cnt, u32 trn, const char* par, void* val)
{
  GLint link = ogl.glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

#ifdef DEBUG
  GLint sid;
  ogl.glGetIntegerv(GL_CURRENT_PROGRAM, &sid);

  //kgm_log() << __FILE__ << " " << __LINE__ << " id: " << par << " link: " << link << " sid: " << sid << "\n";
 #endif

  switch(type)
  {
  case gcunitype_mtx2:
    ogl.glUniformMatrix2fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  case gcunitype_mtx3:
    ogl.glUniformMatrix3fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  case gcunitype_mtx4:
    ogl.glUniformMatrix4fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  }

  //kgm_log() << __FILE__ << " " << __LINE__ << " error: " << ogl.glGetError() << "\n";
}

void kgmOGL::gcUniformSampler(void* s, const char* par, void* val)
{
  GLint link = ogl.glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  ogl.glUniform1i(link, (GLu32)(size_t)val);
}

#ifdef DEBUG
void kgmOGL::gcGetUniform(void* s, const char* par, void* val)
{
  ogl.glGetUniformfv((GLhandle)(size_t)s, (GLint)(size_t)par, (float*)val);
}
#endif

#endif
