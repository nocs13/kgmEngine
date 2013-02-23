#pragma once
#ifdef WIN32
 #include <windows.h>
#elif defined LINUX
 #include <X11/Xlib.h>
 #include <X11/keysym.h>
 #include <X11/extensions/xf86vmode.h>
 #include <GL/glx.h>
#endif

#ifndef ANDROID

#include <GL/gl.h>
#include "inc/glext.h"

#ifdef WIN32
 #define GLEXT_FN(func, type)  func = (type)wglGetProcAddress(#func)
#endif

#ifdef LINUX
 #define GLEXT_FN(func, type)  func = (type)glXGetProcAddress((const GLubyte*)#func)
#endif

//DRAW ELEMENTS
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
//TEXTURE EXTENTIONS
extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB;
//VERTEX BUFFER EXTENTIONS
extern PFNGLGENBUFFERSARBPROC			glGenBuffersARB;
extern PFNGLDELETEBUFFERSARBPROC		glDeleteBuffersARB;
extern PFNGLBINDBUFFERARBPROC			glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC			glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC		glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC		glGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC			glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC			glUnmapBufferARB;
//FRAME BUFFER EXTENTIONS
extern PFNGLGENFRAMEBUFFERSEXTPROC			glGenFramebuffersEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC		glDeleteFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebufferEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC		glFramebufferTexture2DEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	glCheckFramebufferStatusEXT;

//GLSL SHADER EXTENTIONS
extern PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB;
extern PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB;
extern PFNGLSHADERSOURCEARBPROC         glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC        glCompileShaderARB;
extern PFNGLATTACHOBJECTARBPROC         glAttachObjectARB;
extern PFNGLDETACHOBJECTARBPROC         glDetachObjectARB;
extern PFNGLLINKPROGRAMARBPROC          glLinkProgramARB;
extern PFNGLVALIDATEPROGRAMARBPROC      glValidateProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC           glGetInfoLogARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB;
extern PFNGLUNIFORM1IARBPROC            glUniform1iARB;
extern PFNGLUNIFORM1IVARBPROC           glUniform1ivARB;
extern PFNGLUNIFORM2IVARBPROC           glUniform2ivARB;
extern PFNGLUNIFORM3IVARBPROC           glUniform3ivARB;
extern PFNGLUNIFORM4IVARBPROC           glUniform4ivARB;
extern PFNGLUNIFORM1FVARBPROC           glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC           glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC           glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC           glUniform4fvARB;
extern PFNGLUNIFORMMATRIX2FVARBPROC     glUniformMatrix2fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC     glUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC     glUniformMatrix4fvARB;

#endif 

#ifdef WIN32
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
extern  PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;
#endif

#ifdef LINUX
extern PFNGLXSWAPINTERVALSGIPROC glxSwapIntervalSGI;
#endif


bool glInitExt();
