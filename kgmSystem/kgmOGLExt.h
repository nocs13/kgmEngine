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

#ifdef ANDROID
 #define GLEXT_FN(func, type)
#endif


#ifndef GL_FRAMEBUFFER_COMPLETE
 #define GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
#endif

#ifndef GL_FRAMEBUFFER_COMPLETE
 #define GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
#endif

//DRAW ELEMENTS
extern PFNGLDRAWRANGEELEMENTSPROC       glDrawRangeElements;
//TEXTURE EXTENTIONS
extern PFNGLACTIVETEXTUREARBPROC		    glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTexture;
//VERTEX BUFFER EXTENTIONS
extern PFNGLGENBUFFERSARBPROC			glGenBuffers;
extern PFNGLDELETEBUFFERSARBPROC		glDeleteBuffers;
extern PFNGLBINDBUFFERARBPROC			glBindBuffer;
extern PFNGLBUFFERDATAARBPROC			glBufferData;
extern PFNGLBUFFERSUBDATAARBPROC		glBufferSubData;
extern PFNGLGETBUFFERSUBDATAARBPROC		glGetBufferSubData;
extern PFNGLMAPBUFFERARBPROC			glMapBuffer;
extern PFNGLUNMAPBUFFERARBPROC			glUnmapBuffer;
//FRAME BUFFER EXTENTIONS
extern PFNGLGENFRAMEBUFFERSEXTPROC			  glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC		  glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFEREXTPROC			  glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC		glFramebufferTexture2D;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	glCheckFramebufferStatus;

//GLSL SHADER EXTENTIONS
extern PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObject;
extern PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObject;
extern PFNGLDELETEOBJECTARBPROC         glDeleteObject;
extern PFNGLSHADERSOURCEARBPROC         glShaderSource;
extern PFNGLCOMPILESHADERARBPROC        glCompileShader;
extern PFNGLATTACHOBJECTARBPROC         glAttachObject;
extern PFNGLDETACHOBJECTARBPROC         glDetachObject;
extern PFNGLLINKPROGRAMARBPROC          glLinkProgram;
extern PFNGLVALIDATEPROGRAMARBPROC      glValidateProgram;
extern PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObject;
extern PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfv;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv;
extern PFNGLGETINFOLOGARBPROC           glGetInfoLog;
extern PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocation;
extern PFNGLUNIFORM1IARBPROC            glUniform1i;
extern PFNGLUNIFORM1IVARBPROC           glUniform1iv;
extern PFNGLUNIFORM2IVARBPROC           glUniform2iv;
extern PFNGLUNIFORM3IVARBPROC           glUniform3iv;
extern PFNGLUNIFORM4IVARBPROC           glUniform4iv;
extern PFNGLUNIFORM1FVARBPROC           glUniform1fv;
extern PFNGLUNIFORM2FVARBPROC           glUniform2fv;
extern PFNGLUNIFORM3FVARBPROC           glUniform3fv;
extern PFNGLUNIFORM4FVARBPROC           glUniform4fv;
extern PFNGLUNIFORMMATRIX2FVARBPROC     glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVARBPROC     glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVARBPROC     glUniformMatrix4fv;
#endif 

#ifdef WIN32
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
extern  PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;
#endif

#ifdef LINUX
extern PFNGLXSWAPINTERVALSGIPROC glxSwapIntervalSGI;
#endif


bool glInitExt();
