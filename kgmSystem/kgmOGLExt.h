#pragma once
#ifdef WIN32
 #include <windows.h>
 #include <GL/gl.h>
 #include <GL/glext.h>
#elif defined LINUX
 #include <X11/Xlib.h>
 #include <X11/keysym.h>
 #include <X11/extensions/xf86vmode.h>
 #include <GL/glx.h>
 #include <GL/gl.h>
 #include "inc/GL/glext.h"
#elif defined ANDROID
#endif

#ifndef ANDROID


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
extern PFNGLGENFRAMEBUFFERSEXTPROC        glGenFramebuffersEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC     glDeleteFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC        glBindFramebufferEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC   glFramebufferTexture2DEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;

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
extern PFNGLBINDATTRIBLOCATIONARBPROC   glBindAttribLocationARB;

extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
extern PFNGLVERTEXATTRIBPOINTERARBPROC     glVertexAttribPointerARB;

#define glActiveTexture          glActiveTextureARB
#define glClientActiveTexture    glClientActiveTextureARB
#define glGenBuffers             glGenBuffersARB
#define glDeleteBuffers          glDeleteBuffersARB
#define glBindBuffer             glBindBufferARB
#define glBufferData             glBufferDataARB
#define glBufferSubData          glBufferSubDataARB
#define glGetBufferSubData       glGetBufferSubDataARB
#define glMapBuffer              glMapBufferARB
#define glUnmapBuffer            glUnmapBufferARB
#define glGenFramebuffers        glGenFramebuffersEXT
#define glDeleteFramebuffers     glDeleteFramebuffersEXT
#define glBindFramebuffer        glBindFramebufferEXT
#define glFramebufferTexture2D   glFramebufferTexture2DEXT
#define glCheckFramebufferStatus glCheckFramebufferStatusEXT
#define glCreateShaderObject     glCreateShaderObjectARB
#define glCreateProgramObject    glCreateProgramObjectARB
#define glDeleteObject           glDeleteObjectARB
#define glShaderSource           glShaderSourceARB
#define glCompileShader          glCompileShaderARB
#define glAttachObject           glAttachObjectARB
#define glDetachObject           glDetachObjectARB
#define glLinkProgram            glLinkProgramARB
#define glValidateProgram        glValidateProgramARB
#define glUseProgramObject       glUseProgramObjectARB
#define glGetObjectParameterfv   glGetObjectParameterfvARB
#define glGetObjectParameteriv   glGetObjectParameterivARB
#define glGetInfoLog             glGetInfoLogARB
#define glGetUniformLocation     glGetUniformLocationARB
#define glUniform1i              glUniform1iARB
#define glUniform1iv             glUniform1ivARB
#define glUniform2iv             glUniform2ivARB
#define glUniform3iv             glUniform3ivARB
#define glUniform4iv             glUniform4ivARB
#define glUniform1fv             glUniform1fvARB
#define glUniform2fv             glUniform2fvARB
#define glUniform3fv             glUniform3fvARB
#define glUniform4fv             glUniform4fvARB
#define glUniformMatrix2fv       glUniformMatrix2fvARB
#define glUniformMatrix3fv       glUniformMatrix3fvARB
#define glUniformMatrix4fv       glUniformMatrix4fvARB
#define glBindAttribLocation     glBindAttribLocationARB
#define glDisableVertexAttribArray glDisableVertexAttribArrayARB
#define glEnableVertexAttribArray glEnableVertexAttribArrayARB
#define glVertexAttribPointer     glVertexAttribPointerARB
#endif

#ifdef WIN32
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
extern  PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;
#endif

#ifdef LINUX
extern PFNGLXSWAPINTERVALSGIPROC glxSwapIntervalSGI;
#endif

bool glInitExt();
