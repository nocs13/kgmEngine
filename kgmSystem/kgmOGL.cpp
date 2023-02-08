// kgmOGL.cpp: implementation of the kgmOGL class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmOGL.h"
#include "kgmOGLExt.h"
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

#define GL_DEBUG_OUTPUT_KHR               0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR   0x8242

static void __onDebugMessage(GLenum source,GLenum type,GLint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam)
{
  kgm_log() << "OGL Debug log: " << message << ".\n";
  //fprintf(stderr, "OGL Debug log: %s.\n", message);
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

  if (!m_glctx && wnd->m_visual)
      m_glctx = glXCreateContext(wnd->m_dpy, wnd->m_visual, 0, GL_TRUE);

  if (!m_glctx)
  {
    kgm_log() << "Error: Cannot create ogl context.\n";

    m_error = 1;

    return;
  }

  if(!glXMakeCurrent(wnd->m_dpy, wnd->m_wnd, m_glctx))
  {
    kgm_log() << "Error: Cannot activate ogl context.\n";

    m_error = 1;

    return;
  }

  if(glXIsDirect(wnd->m_dpy, m_glctx))
    kgm_log() << "Direct Rendering!\n";
  else
    kgm_log() << "Not direct Rendering!\n";

#endif

#ifdef DEBUG
  kgm_log() << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n";
  kgm_log() << "GLSL   Version: " << (char*) glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
  kgm_log() << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
  kgm_log() << "OpenGL Render: " << (char*) glGetString(GL_RENDERER) << "\n";
#endif


  glInitExt();

  glEnable(GL_TEXTURE_CUBE_MAP);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glShadeModel(GL_SMOOTH);

  const GLubyte* ext = glGetString(GL_EXTENSIONS);

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


#ifdef DEBUG

  if (glDebugMessageCallbackKHR != null)
  {
    kgm_log() << "Enabling GL Debuging.\n";

    glEnable(GL_DEBUG_OUTPUT_KHR);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);

    glDebugMessageCallbackKHR( (GLDEBUGPROCKHR) __onDebugMessage, this);

    kgm_log() << "Enabling GL Debuging status " << glGetError() << ".\n";
  }

#endif

#ifdef GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB
  glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &g_num_compressed_format);

  g_compressed_format = (GLint*)kgm_alloc(g_num_compressed_format * sizeof(GLint));

  glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, g_compressed_format);

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

  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_image_units);

  //init local values
  glEnable(GL_TEXTURE_2D);

  glPolygonOffset (1.0f, 1.0f);

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
    if(!glXMakeCurrent(m_wnd->m_dpy, None, NULL))
      kgmLog::log("Could not release drawing context.\n");

    glXDestroyContext(m_wnd->m_dpy, m_glctx);

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
      glEnable(GL_BLEND);
    else
      glDisable(GL_BLEND);
    break;
  case gcpar_culling:
    if(value)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);
    break;
  case gcpar_textures:
    if(value)
      glEnable(GL_TEXTURE_2D);
    else
      glDisable(GL_TEXTURE_2D);
    break;
  case gcpar_depth:
    if(value)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
    break;
#ifdef GL_ALPHA_TEST
  case gcpar_alpha:
    if(value)
      glEnable(GL_ALPHA_TEST);
    else
      glDisable(GL_ALPHA_TEST);
    break;
#endif
  case gcpar_stencil:
    if(value)
      glEnable(GL_STENCIL_TEST);
    else
      glDisable(GL_STENCIL_TEST);
    break;
  case gcpar_colormask:
  {
    char* c = (char*)&value;
    GLboolean r = (c[0])?(GL_TRUE):(GL_FALSE),
        g = (c[1])?(GL_TRUE):(GL_FALSE),
        b = (c[2])?(GL_TRUE):(GL_FALSE),
        a = (c[3])?(GL_TRUE):(GL_FALSE);
    glColorMask(r, g, b, a);
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
        glEnable(GL_CLIP_PLANE0 + cp->id);
        glClipPlane(GL_CLIP_PLANE0 + cp->id, pl);
      }
      else
      {
        glDisable(GL_CLIP_PLANE0 + cp->id);
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

    glClearColor(r, g, b, a);
  }

  if(flag & gcflag_depth)
  {
    cl |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(depth);
  }

  if(flag & gcflag_stencil)
  {
    cl |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(sten);
  }

  glClear(cl);
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

  glXSwapBuffers(m_wnd->m_dpy, m_wnd->m_wnd);

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
  glViewport(x, y, w, h);
}


//BLEND
void  kgmOGL::gcBlend(bool en, u32 eq, u32 fnsrc, u32 fndst)
{
  if(!en)
  {
    glDisable(GL_BLEND);

    return;
  }

  switch(eq)
  {
  case gcblend_eqadd:
    glBlendEquation(GL_FUNC_ADD_EXT);
    break;
  case gcblend_eqsub:
    glBlendEquation(GL_FUNC_SUBTRACT_EXT);
    break;
  case gcblend_eqmin:
    glBlendEquation(GL_MIN);
    break;
  case gcblend_eqmax:
    glBlendEquation(GL_MAX);
    break;
  case gcblend_eqrsub:
    glBlendEquation(GL_FUNC_REVERSE_SUBTRACT_EXT);
    break;
  default:
    glBlendEquation(GL_FUNC_ADD);
  }

  glBlendFunc(gl_enum(fnsrc), gl_enum(fndst));
  glEnable(GL_BLEND);
}

//CULL
void kgmOGL::gcCull(u32 mode)
{
  if(!mode)
  {
    glDisable(GL_CULL_FACE);

    return;
  }

  switch(mode)
  {
  case gccull_back:
    glCullFace(GL_BACK);
    break;
  case gccull_front:
    glCullFace(GL_FRONT);
    break;
  }

  glEnable(GL_CULL_FACE);
}

//DEPTH
void kgmOGL::gcDepth(bool depth, bool mask, u32 mode)
{
  if(depth)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  if(mask)
    glDepthMask(GL_TRUE);
  else
    glDepthMask(GL_FALSE);

  glDepthFunc(gl_enum(mode));
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
  case gctex_fmtdepten:
    pic_fmt = GL_DEPTH_STENCIL_EXT;
    int_fmt = GL_DEPTH_STENCIL_EXT;
    break;
  }

  u8* cnt = null;

  switch(type)
  {
  case gctype_tex2d:
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    break;
  case gctype_texcube:
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
    break;
  }

  switch(type)
  {
  case gctype_tex2d:
    if(fmt == gctex_fmtdepth)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, pd);
#ifdef DEBUG
    if (glGetError() != GL_NO_ERROR)
      kgm_log() << "Error: Cannot create 2D texture. eid=" << (s32) glGetError() << ".\n";
#endif
    break;
  case gctype_texcube:
    cnt = (u8*) kgm_alloc(bpc * w * h);
    memset(cnt, 0xff, bpc * w * h);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, int_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, cnt);
    kgm_free(cnt);
    break;
  }

  switch(type)
  {
  case gctype_texcube:
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    break;
  default:
    glBindTexture(GL_TEXTURE_2D, 0);
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
    glDeleteTextures(1, &tex);


#ifdef DEBUG
  kgm_log() << "Free texture: " << (s32) tex << "\n";
#endif
}

void kgmOGL::gcSetTexture(u32 stage, gchandle t)
{
  if(!t)
  {
    glActiveTexture(GL_TEXTURE0 + stage);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (glGetError() == GL_INVALID_OPERATION)
    {
      glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

      if (glGetError() != GL_NO_ERROR)
      {
        int k = 0;
      }
    }

    return;
  }

  glActiveTexture(GL_TEXTURE0 + stage);
  glBindTexture(GL_TEXTURE_2D, (GLu32) (size_t) t);

  if (glGetError() == GL_INVALID_OPERATION)
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, (GLu32) (size_t) t);

    if (glGetError() != GL_NO_ERROR)
    {
      int k = 0;
    }
  }
}

// TARGET
gchandle kgmOGL::gcGenTarget(u32 w, u32 h, bool d, bool s)
{
  GLu32 buffer = 0;
  glGenFramebuffers(1, &buffer);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, buffer);
  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

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
    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER_EXT, depth);

    if (s)
    {
      glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL_EXT, w, h);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
    }
    else
    {
      glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
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
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

  if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
  {
    kgm_log() << "Error: cannot use framebuffer object!\n";
    return null;
  }
#endif

  glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);

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

  glBindFramebuffer(GL_FRAMEBUFFER_EXT, rb->frame);
  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  glBindRenderbuffer(GL_RENDERBUFFER_EXT, rb->depth);

  switch (type)
  {
  case gctype_texdepth:
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D,  (GLint) (size_t) tex, 0);
    break;
  case gctype_tex2d:
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, (GLint) (size_t) tex, 0);
    break;
  case gctype_texcube:
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_cubemapside, (GLint) (size_t) tex, 0);

    ++rb->cmside;

    if (rb->cmside > 5)
      rb->cmside = 0;
    break;
  default:
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, (GLint)            0, 0);
  };

  rb->type = type;
  rb->color = (GLint) (size_t) tex;

#ifdef DEBUG
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

  if (status != GL_FRAMEBUFFER_COMPLETE) {
    kgm_log() << "Error: Attaching texture to framebuffer not completed.\n";
    kgm_log() << "Error: " << (s32)status << ".\n";
  }
#endif

  glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);

  return true;
}

void kgmOGL::gcFreeTarget(gchandle t)
{
  RenderBuffer* rb = static_cast<RenderBuffer*>(t);

  if(!rb)
    return;

  if (rb->depth)
    glDeleteRenderbuffers(1, &rb->depth);

  if (rb->frame)
    glDeleteFramebuffers(1, &rb->frame);

  kgm_free(rb);
}

void kgmOGL::gcSetTarget(gchandle t)
{
  RenderBuffer* rb = (RenderBuffer*) t;

  if(!rb || !rb->frame)
  {
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
    glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);

    m_renderbuffer = null;

    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER_EXT, rb->frame);
  //glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  //glBindRenderbuffer(GL_RENDERBUFFER_EXT, rb->depth);

  m_renderbuffer = rb;

#ifdef DEBUG
  GLenum err = glGetError();

  if (err != GL_NO_ERROR) {
    kgm_log() << "Error: Cannot set framebuffer.\n";
    kgm_log() << "Error: Eid is " << (s32) err << ".\n";
  }
#endif
}

//CLIP PLANE
void kgmOGL::gcClipPlane(bool en, u32 id, f64 plane[4])
{
  if (!en)
  {
    glDisable(GL_CLIP_PLANE0 + id);
  }
  else
  {
    glEnable(GL_CLIP_PLANE0 + id);
    glClipPlane(GL_CLIP_PLANE0 + id, plane);
  }
}

//STENCIL
void kgmOGL::gcStencil(bool en, u32 func, u32 mask, u32 ref,
               u32 fail, u32 zfail, u32 zpass)
{

}

//SCISSOR
void kgmOGL::gcScissor(bool on, int x, int y, int w, int h)
{
  glScissor(x, y, w, h);

  if (on)
    glEnable(GL_SCISSOR_TEST);
  else
    glDisable(GL_SCISSOR_TEST);
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

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(v_fmt & gcv_xyz)
  {
    ah = glGetAttribLocation(m_shader, "a_Vertex");

    if(ah != -1)
    {
      glVertexAttribPointer(ah, 3, GL_FLOAT, GL_FALSE, v_size, pM);
      glEnableVertexAttribArray(ah);
    }

    pM += (sizeof(float) * 3);
  }

  if(v_fmt & gcv_nor)
  {
    ah = glGetAttribLocation(m_shader, "a_Normal");

    if(ah != -1)
    {
      glEnableVertexAttribArray(ah);
      glVertexAttribPointer(ah, 3, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (sizeof(float)*3);
  }

  if(v_fmt & gcv_col)
  {
    ah = glGetAttribLocation(m_shader, "a_Color");

    if(ah != -1)
    {
      glEnableVertexAttribArray(ah);
      glVertexAttribPointer(ah, 4, GL_FLOAT, GL_TRUE, v_size, pM);
    }

    pM += (sizeof(float) * 4);
  }

  if(v_fmt & gcv_uv0)
  {
    ah = glGetAttribLocation(m_shader, "a_UV");

    if(ah != -1)
    {
      glEnableVertexAttribArray(ah);
      glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_uv1)
  {
    ah = glGetAttribLocation(m_shader, "a_UV2");
    if(ah != -1)
    {
      glEnableVertexAttribArray(ah);
      glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_uv2)
  {
    ah = glGetAttribLocation(m_shader, "a_UV3");

    if(ah != -1)
    {
      glEnableVertexAttribArray(ah);
      glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
    }

    pM += (uv_size);
  }

  if(v_fmt & gcv_bn0)
  {
    ah = glGetAttribLocation(m_shader, "a_Weights");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 4, GL_FLOAT, GL_FALSE, v_size, pM);
    pM += (4 * sizeof(float));
    ah = glGetAttribLocation(m_shader, "a_Indices");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 4, GL_FLOAT, GL_FALSE, v_size, pM);
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
      //glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_INT, i_pnt);
      glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_INT, i_pnt);
      break;
    default:
      //glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_SHORT,i_pnt);
      glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_SHORT, i_pnt);
    }
  }
  else
  {
    glDrawArrays(gl_enum(pmt), 0, v_cnt);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);
  glDisableVertexAttribArray(5);
}

//VERTEX & INDEX BUFFER
void* kgmOGL::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize)
{
  VBO* vbo = new VBO;
  memset(vbo, 0, sizeof(VBO));

  if(vdata && vsize)
  {
    glGenBuffers(1, &vbo->vb);
    glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
    glBufferData(GL_ARRAY_BUFFER, vsize, vdata, GL_STATIC_DRAW);
    vbo->vsize = vsize;
  }

  if(idata && isize)
  {
    glGenBuffers(1, &vbo->ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, idata, GL_STATIC_DRAW);
    vbo->isize = isize;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return (void*)vbo;
}

void  kgmOGL::gcFreeVertexBuffer(void* b)
{
  VBO* vbo = (VBO*)b;

  if(!vbo)
    return;

  if(vbo->vb)
    glDeleteBuffers(1, &vbo->vb);

  if(vbo->ib)
    glDeleteBuffers(1, &vbo->ib);

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
    glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
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
    al = glGetAttribLocation(m_shader, "a_Vertex");

    if(al != -1)
    {
      glEnableVertexAttribArray(al);
      glVertexAttribPointer(al, 3, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 3;
    }
  }

  if(vfmt & gcv_nor)
  {
    al = glGetAttribLocation(m_shader, "a_Normal");

    if(al != -1)
    {
      glEnableVertexAttribArray(al);
      glVertexAttribPointer(al, 3, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 3;
    }
  }

  if(vfmt & gcv_col)
  {
    al = glGetAttribLocation(m_shader, "a_Color");

    if(al != -1)
    {
      glEnableVertexAttribArray(al);
      glVertexAttribPointer(al, 4, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * 4;
    }
  }

  if(vfmt & gcv_uv0)
  {
    al = glGetAttribLocation(m_shader, "a_UV");

    if(al != -1)
    {
      glEnableVertexAttribArray(al);
      glVertexAttribPointer(al, 2, GL_FLOAT, GL_FALSE, vsize, (void*)offset);
      offset = offset + sizeof(float) * uvt;
    }
  }

  if(vfmt & gcv_uv1)
  {
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv2)
  {
    glClientActiveTexture(GL_TEXTURE2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv3)
  {
    glClientActiveTexture(GL_TEXTURE3);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv4)
  {
    glClientActiveTexture(GL_TEXTURE4);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv5)
  {
    glClientActiveTexture(GL_TEXTURE5);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv6)
  {
    glClientActiveTexture(GL_TEXTURE6);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv7)
  {
    glClientActiveTexture(GL_TEXTURE7);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vbo->ib && icnt)
  {
    //  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);

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
      glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_INT, BUFFER_OFFSET(ioff));
      //glDrawRangeElements (gl_enum(pmt), 0, vcnt - 1, icnt, GL_UNSIGNED_INT, ((char*)0L + ioff));
      break;
    default:
      //glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);
      glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_SHORT, BUFFER_OFFSET(ioff));
      //glDrawRangeElements (gl_enum(pmt), ioff, ioff + icnt, icnt/3, GL_UNSIGNED_SHORT, (void*)0);
    }
  }
  else
  {
    glDrawArrays(gl_enum(pmt), 0, vcnt);
  }

  glBindBuffer( GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

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

  prog = glCreateProgramObject();

  gerr = glGetError();

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
    vshad = glCreateShaderObject(GL_VERTEX_SHADER);
    glShaderSource(vshad, 1, (const GLcharARB**)&ptr, &size);
    glCompileShader(vshad);
    glGetObjectParameteriv(vshad, GL_OBJECT_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE)
    {
#ifdef DEBUG
      glGetInfoLog(vshad, sizeof(tbuf), &size, tbuf);
      kgm_log() << "gcGenShader: VShader> " << (char*)tbuf << " " << (s32)strlen(tbuf) << "\n";
#endif

      glDeleteObject(vshad);
      vshad = 0;
    }
    else
    {
      glAttachObject(prog, vshad);
    }
  }

  if(fsrc.data() && fsrc.length())
  {
    const char *ptr = (const char*) fsrc.data();

    size = strlen(ptr);
    fshad = glCreateShaderObject(GL_FRAGMENT_SHADER);
    glShaderSource(fshad, 1, (const GLcharARB**)&ptr, NULL);
    glCompileShader(fshad);
    glGetObjectParameteriv(fshad, GL_OBJECT_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE)
    {
#ifdef DEBUG
      glGetInfoLog(fshad, sizeof(tbuf), &size, tbuf);
      kgm_log() << "gcGenShader: FShader> " << (char*)tbuf << " " << (s32)strlen(tbuf) << "\n";
#endif

      glDeleteObject(vshad);
      fshad = 0;
    }
    else
    {
      glAttachObject(prog, fshad);
    }
  }

  //glBindAttribLocation((GLhandle)prog, 0, "g_Vertex");
  //glBindAttribLocation((GLhandle)prog, 1, "g_Normal");
  //glBindAttribLocation((GLhandle)prog, 2, "g_Color");
  //glBindAttribLocation((GLhandle)prog, 3, "g_Texcoord");

  glLinkProgram(prog);

  stat[0] = 0;

  glGetObjectParameteriv(prog, GL_LINK_STATUS, stat);

  if(stat[0] == GL_FALSE)
  {
#ifdef DEBUG
    glGetInfoLog(prog, sizeof(tbuf), &size, tbuf);
    kgm_log() << "gcGenShader: Link log> " << tbuf << "\n";
#endif

    glDeleteObject(vshad);
    glDeleteObject(fshad);
    glDeleteObject(prog);

    return null;
  }

  glValidateProgram(prog);

  stat[0] = 0;

  glGetObjectParameteriv(prog, GL_VALIDATE_STATUS, stat);

  if(stat[0] == GL_FALSE)
  {
#ifdef DEBUG
    glGetInfoLog(prog, sizeof(tbuf), &size, tbuf);
    kgm_log() << "gcGenShader: Validate log> " << tbuf << "\n";
#endif

    glDeleteObject(vshad);
    glDeleteObject(fshad);
    glDeleteObject(prog);

    return null;
  }

  kgm_log() << "gcGenShader: program id " << (s32) prog << ".\n";

  return (gchandle) (size_t) prog;
}

void kgmOGL::gcFreeShader(gchandle s)
{
  if(glIsProgram((GLint) (size_t) s) != GL_TRUE)
  {
#ifdef DEBUG
    kgm_log() << "gcFreeShader: Invalid shader program " << (s32) (size_t) s << "\n";
#endif
    return;
  }

#ifdef DEBUG
  kgm_log() << "gcFreeShader: shader program id " << (s32) (size_t) s << ".\n";
#endif

  glDeleteObject((GLint) (size_t) s);
}

void kgmOGL::gcSetShader(gchandle s)
{
  if (s == null)
  {
    glUseProgramObject(0);

    return;
  }

  GLint prog = 0;

  glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

  if (prog == (GLint) (size_t) s)
  {
    return;
  }

  glUseProgramObject((GLhandle)(size_t)s);

  m_shader = (GLhandle)(size_t)s;
}

void  kgmOGL::gcBindAttribute(void* s, int i, const char* attr)
{
  if(s)
  {
    glBindAttribLocation((GLhandle)(size_t)s, i, attr);
  }
}

void kgmOGL::gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val)
{
  GLint link = glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  switch(type){
  case gcunitype_float1:
    glUniform1fv(link, cnt, (float*)val);
    break;
  case gcunitype_float2:
    glUniform2fv(link, cnt, (float*)val);
    break;
  case gcunitype_float3:
    glUniform3fv(link, cnt, (float*)val);
    break;
  case gcunitype_float4:
    glUniform4fv(link, cnt, (float*)val);
    break;
  case gcunitype_int1:
    glUniform1iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int2:
    glUniform2iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int3:
    glUniform3iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int4:
    glUniform4iv(link, cnt, (const int*)val);
    break;
  }

#ifdef DEBUG
  GLenum error = glGetError();

  if (glGetError() != GL_NO_ERROR)
  {
    fprintf(stderr, "Error glUniform: %d.\n", error);
  }
#endif
}

void kgmOGL::gcUniformMatrix(void* s, u32 type, u32 cnt, u32 trn, const char* par, void* val)
{
  GLint link = glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  switch(type)
  {
  case gcunitype_mtx2:
    glUniformMatrix2fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  case gcunitype_mtx3:
    glUniformMatrix3fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  case gcunitype_mtx4:
    glUniformMatrix4fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  }
}

void kgmOGL::gcUniformSampler(void* s, const char* par, void* val)
{
  GLint link = glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  glUniform1i(link, (GLu32)(size_t)val);
}

#ifdef DEBUG
void kgmOGL::gcGetUniform(void* s, const char* par, void* val)
{
  glGetUniformfv((GLhandle)(size_t)s, (GLint)(size_t)par, (float*)val);
}
#endif

#endif