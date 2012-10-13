// kgmOGL.cpp: implementation of the kgmOGL class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmOGL.h"
#include "kgmOGLExt.h"
#include "kgmOGLWindow.h"

#ifdef WIN32
 #pragma comment(lib, "opengl32.lib")
 #pragma comment(lib, "glu32.lib")
#endif

GLu32          m_rendertarget = 0;

inline void framebuffer_status(GLu32 stat){
   switch(stat){
   case GL_FRAMEBUFFER_COMPLETE_EXT:
	   break;
   case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
	   stat = 0;
	   break;
   case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
	   stat = 0;
	   break;
   case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
	   stat = 0;
	   break;
   case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
	   stat = 0;
	   break;
   case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
	   stat = 0;
	   break;
   case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
	   stat = 0;
	   break;
   case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
	   stat = 0;
	   break;
   case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
	   stat = 0;
	   break;
   }
}

kgmOGL::kgmOGL(kgmOGLWindow *wnd){
 if(!wnd)                         
  return;
  
 this->m_wnd = wnd;
 glInitExt();
 glEnable(GL_ACTIVE_TEXTURE_ARB);
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LEQUAL);
 glEnable(GL_CULL_FACE);
 glCullFace(GL_BACK);
 glShadeModel(GL_SMOOTH);

 glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
 glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  const GLubyte* ext = glGetString(GL_EXTENSIONS);
  /*FILE* out = fopen("glExt.txt", "w");
  for(int i = 0; i < strlen((char*)ext); i++){
   if(ext[i] == ' ')
	   fprintf(out, "\n");
   else
	   fprintf(out, "%c", ext[i]);
  }
  fclose(out);*/

#ifdef WIN32
 if(wglSwapIntervalEXT) wglSwapIntervalEXT(0);
#endif
#ifdef LINUX
// if(glxSwapIntervalSGI) glxSwapIntervalSGI(0);
#endif
//init local values

 glEnable(GL_TEXTURE_2D);
 m_renderbuffer = 0;
}

kgmOGL::~kgmOGL(){
 m_rendertarget = 0;
}

void kgmOGL::gcSetParameter(u32 param, void* value){
 switch(param)
 {
 case gcpar_lighting:
	 if(value) 
		 glEnable(GL_LIGHTING);
	 else 
		 glDisable(GL_LIGHTING);
	 break;
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
 case gcpar_alpha:
	 if(value) 
		 glEnable(GL_ALPHA_TEST);
	 else 
		 glDisable(GL_ALPHA_TEST);
	 break;
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
 }
}
void kgmOGL::gcGetParameter(u32 param, void* value){

}
void kgmOGL::gcClear(u32 flag, u32 col, float depth, u32 sten){
 GLu32 cl = 0;		
 
 if(flag & gcflag_color){
  cl |= GL_COLOR_BUFFER_BIT;
  float inv = 1.0f / 255.0f;
  float a = inv * ((col & 0xff000000) >> 24),
        r = inv * ((col & 0x00ff0000) >> 16),
        g = inv * ((col & 0x0000ff00) >> 8),
	    b = inv *  (col & 0x000000ff);
  glClearColor(r, g, b, a);
 }

 if(flag & gcflag_depth){
  cl |= GL_DEPTH_BUFFER_BIT;
  glClearDepth(depth);
 }

 if(flag & gcflag_stencil){
  cl |= GL_STENCIL_BUFFER_BIT;
  glClearStencil(sten);
 }
 glClear(cl);
}

void kgmOGL::gcBegin(){
}

void kgmOGL::gcEnd(){
}

void kgmOGL::gcRender(){
#ifdef WIN32
 SwapBuffers(m_wnd->m_hdc);
#endif
#ifdef LINUX
 glXSwapBuffers(m_wnd->m_dpy, m_wnd->m_wnd);
#endif
}

void kgmOGL::gcSetTarget(void* t){
 if(!t || !((Texture*)t)->buffer){
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  return;
 }

 glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ((Texture*)t)->buffer);
 GLenum err = glGetError();
 if(err != GL_NO_ERROR){
  int k = 0;
 }
 GLint ipar = 0;
 glEnable(GL_DEPTH_TEST);
 glDepthMask(true);
 glDepthFunc(GL_LEQUAL);
 glGetIntegerv(GL_DEPTH_BITS, &ipar);
 if(ipar == 0){
  int k = 0;
 }
//  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
//  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
//  glDrawBuffer(GL_BACK);
//  glReadBuffer(GL_BACK);
}

void kgmOGL::gcSetMatrix(u32 mode, float* mtx){
 switch(mode){
 case gcmtx_proj:
	  glMatrixMode(GL_PROJECTION);
  break;
 case gcmtx_view:
	  glMatrixMode(GL_MODELVIEW);
  break;
 case gcmtx_text:
	  glMatrixMode(GL_TEXTURE);
  break;
 }
 glLoadMatrixf(mtx);
}

void kgmOGL::gcGetMatrix(u32 mode, float* mtx){
 switch(mode){
 case gcmtx_proj:
	  glGetFloatv(GL_PROJECTION_MATRIX, mtx);
  break;
 case gcmtx_view:
	  glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
  break;
 case gcmtx_text:
	  glGetFloatv(GL_TEXTURE_MATRIX, mtx);
  break;
 }
}

void kgmOGL::gcSetViewport(int x, int y, int w, int h, float n, float f){
 glViewport(x, y, w, h);
 glDepthRange(n, f);
}

//Light
/*
void kgmOGL::gcSetLight(int i, kgmLight *light){
 if(i > GL_MAX_LIGHTS)
	 return;

 if(!light && (i >= 0)){
  if(glIsEnabled(GL_LIGHT0 + i))
   glDisable(GL_LIGHT0 + i);
  return;
 }

 if(!glIsEnabled(GL_LIGHT0 + i))
  glEnable(GL_LIGHT0 + i);


 glLightfv(GL_LIGHT0 + i, GL_POSITION, (float*)&light->position);
 glLightfv(GL_LIGHT0 + i, GL_AMBIENT,  (float*)&light->ambient);
 glLightfv(GL_LIGHT0 + i, GL_DIFFUSE,  (float*)&light->diffuse);
 glLightfv(GL_LIGHT0 + i, GL_SPECULAR, (float*)&light->specular);
 glLightf(GL_LIGHT0  + i, GL_CONSTANT_ATTENUATION, 1.0f);
// glLightf(GL_LIGHT0  + i, GL_CONSTANT_ATTENUATION, 1.0f / (light->range));
 glLightf(GL_LIGHT0  + i, GL_LINEAR_ATTENUATION, 1.0f / (light->range));
}
*/
//Material
/*
void kgmOGL::gcSetMaterial(kgmMaterial* m){
 if(!m){
  glColor4ub(0xff, 0xff, 0xff, 0xff);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH);
  gcSetTexture(0, 0);
  gcSetTexture(1, 0);
  gcSetTexture(2, 0);
  gcSetTexture(3, 0);
  gcSetTexture(4, 0);
  gcSetTexture(5, 0);
  gcSetTexture(6, 0);
  gcSetTexture(7, 0);
  return;
 }
 if(m->m_flags & kgmMaterial::MF_Blend){
  if(m->m_blend) 
	  glEnable(GL_BLEND);
  else 
	  glDisable(GL_BLEND);
  glBlendFunc(gl_enum(m->m_blend_src), gl_enum(m->m_blend_dst));
 }
 if(m->m_flags & kgmMaterial::MF_Alpha){
  if(m->m_alpha){
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER, .6f);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
  }else
   glDisable(GL_ALPHA_TEST);
 }
 if(m->m_flags & kgmMaterial::MF_Cull){
  if(m->m_cull)
	  glEnable(GL_CULL_FACE);
  else
	  glDisable(GL_CULL_FACE);
 }
 if(m->m_flags & kgmMaterial::MF_Depth){
  if(m->m_depth)
	  glEnable(GL_DEPTH);
  else
	  glDisable(GL_DEPTH);
 }
 if(m->m_flags & kgmMaterial::MF_Material){
  vec4  vc(1, 1, 1, 1);
  m->ambient.get(vc);
  glMaterialfv(GL_FRONT, GL_AMBIENT, (float*)&vc);
  m->diffuse.get(vc);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, (float*)&vc);
  m->specular.get(vc);
  glMaterialfv(GL_FRONT, GL_SPECULAR, (float*)&vc);
  m->emision.get(vc);
  glMaterialfv(GL_FRONT, GL_EMISSION, (float*)&vc);
  glMaterialf(GL_FRONT, GL_SHININESS, m->shininess);
 }
 if(m->m_flags & kgmMaterial::MF_Textures){
  gcSetTexture(0, (m->m_texbase)?(m->m_texbase):(0));
  gcSetTexture(1, (m->m_texbump)?(m->m_texbump):(0));
  gcSetTexture(2, (m->m_texlight)?(m->m_texlight):(0));
  gcSetTexture(3, (m->m_texshadow)?(m->m_texshadow):(0));
 }
}
*/

//FOG
/*
void kgmOGL::gcFog(kgmFog* fog){
 if(!fog){
  glDisable(GL_FOG);
  return;
 }

 glFogi(GL_FOG_MODE, (fog->m_mode)?(GL_EXP):(GL_LINEAR));
 glFogf(GL_FOG_DENSITY, fog->m_density);
 glFogf(GL_FOG_START, fog->m_start);
 glFogf(GL_FOG_END, fog->m_end);
 glFogfv(GL_FOG_COLOR, (float*)&fog->m_color);
 glEnable(GL_FOG);
}
*/
//BLEND
void  kgmOGL::gcBlend(bool en, u32 fnsrc, u32 fndst){
 if(!en){
  glDisable(GL_BLEND);
  return;
 }
 glBlendFunc(gl_enum(fnsrc), gl_enum(fndst));
// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glEnable(GL_BLEND);
}

//ALPHA
void  kgmOGL::gcAlpha(bool en, u32 fn, float ref){
 if(!en){
  glDisable(GL_ALPHA_TEST); 
  return;
 }
 glAlphaFunc(gl_enum(fn), ref);
 glEnable(GL_ALPHA_TEST);
}

//CULL
void kgmOGL::gcCull(u32 mode){
 if(!mode){
  glDisable(GL_CULL_FACE);
  return;
 }
 switch(mode){
 case gccull_back:
	 glCullFace(GL_BACK);
	 break;
 case gccull_front:
	 glCullFace(GL_FRONT);
	 break;
 }
}
//DEPTH
void kgmOGL::gcDepth(bool depth, bool mask, u32 mode){
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


//Dimension MODE
void kgmOGL::gc2DMode(){
 int  x, y, w, h;
 
 m_wnd->getRect(x, y, w, h);
 glMatrixMode(GL_PROJECTION);
  glGetFloatv(GL_PROJECTION_MATRIX, mtx_mode_proj); 
   glLoadIdentity();
    glOrtho(0, w, h, 0, 1, -1);
 glMatrixMode(GL_MODELVIEW);
  glGetFloatv(GL_MODELVIEW_MATRIX, mtx_mode_view); 
   glLoadIdentity();
}
void kgmOGL::gc3DMode()
{
 glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(mtx_mode_proj);
 glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(mtx_mode_view);
}

//TEXTURE
void* kgmOGL::gcGenTexture(void *pd, u32 w, u32 h, u32 fmt, u32 type)
{
 GLu32 tex = 0, frame = 0;
 GLenum pic_fmt;
 GLu32 fmt_bt = 0;

 switch(fmt){
 case gctex_fmt8:
	 pic_fmt = GL_RED;
	 fmt_bt = 1;
	 break;
 case gctex_fmt16:
	 pic_fmt = GL_RGB;
	 fmt_bt = 2;
	 break;
 case gctex_fmt24:
	 pic_fmt = GL_BGR_EXT;
	 fmt_bt = 3;
	 break;
 case gctex_fmt32:
	 pic_fmt = GL_BGRA_EXT;
	 fmt_bt = 4;
	 break;
 case gctex_fmtdepth:
	 pic_fmt = GL_DEPTH_COMPONENT;
	 fmt_bt = GL_DEPTH_COMPONENT;
	 break;
 default:
	 pic_fmt = GL_RGB;
	 fmt_bt = 3;
 }

 switch(type){
 case gctype_tex2d:
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  break;
 case gctype_texcube:
  glEnable(GL_TEXTURE_CUBE_MAP_ARB);
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, tex);
  break;
 case gctype_textarget:
  glGenFramebuffersEXT(1, &frame); 
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frame);
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
 default:
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
 }

 if(tex == 0){
  return 0;
 }


 const GLubyte* p_str;
 GLenum err = glGetError();

 switch(type){
 case gctype_tex2d:
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if(fmt == gctex_fmtdepth){
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, fmt_bt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, pd);
  break;
// case 1:
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

// glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);

// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, GL_TRUE);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_OPERATOR_SGIX, GL_TEXTURE_LEQUAL_R_SGIX);
 
// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, 0.5);
// glTexGeni(GL_TEXTURE_2D, GL_S, GL_EYE_LINEAR);
// glTexGeni(GL_TEXTURE_2D, GL_T, GL_EYE_LINEAR);
// glTexGeni(GL_TEXTURE_2D, GL_R, GL_EYE_LINEAR);
// glTexGeni(GL_TEXTURE_2D, GL_Q, GL_EYE_LINEAR);
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0,
//              GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
//  break;
 case gctype_texcube:
   glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
   glDisable(GL_TEXTURE_CUBE_MAP_ARB);
  break;
 case gctype_textarget:
  if(fmt == gctex_fmtdepth){
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
   glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_INT, NULL);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
//  glDrawBuffer(GL_NONE);
//  glReadBuffer(GL_NONE);
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, tex, 0);
  }else{
   glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, NULL);
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
  }
  break;
 default:
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, fmt_bt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, pd);
 }

 err = glGetError();
 if(GL_NO_ERROR != err){
	 u32 k = 0;
	 switch(err){
	 case GL_INVALID_ENUM:
		 k = 1;
		 break;
	 case GL_INVALID_VALUE:
		 k = 2;
		 break;
	 case GL_INVALID_OPERATION:
		 k = 3;
		 break;
	 }
 }
 glBindTexture(GL_TEXTURE_2D, 0);
 glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

 Texture* t = new Texture;
 t->width = w;
 t->height = h;
 t->texture = tex;
 t->buffer = frame;
 t->format = fmt;
 t->type = type;
 return (void*)t;
}

void kgmOGL::gcFreeTexture(void *t){
 if(!t)
  return;
 if(((Texture*)t)->texture)
  glDeleteTextures(1,&((Texture*)t)->texture); 
 if(((Texture*)t)->buffer)
  glDeleteFramebuffersEXT(1, &((Texture*)t)->buffer);
 delete (Texture*)t;
}

void kgmOGL::gcSetTexture(u32 stage, void* t){
 if(!t){
  glActiveTextureARB(GL_TEXTURE0_ARB + stage);
  glBindTexture(GL_TEXTURE_2D, 0);
  return;
 }
 glActiveTextureARB(GL_TEXTURE0_ARB + stage);
 glBindTexture(GL_TEXTURE_2D, ((Texture*)t)->texture);
}

/*
//RENDER BUFFER 
void* kgmOGL::gcGenRenderTarget(u32 w, u32 h, u32 flags){
 GLenum stat = 0;
 RenderBuffer* rb = new RenderBuffer;
 memset(rb, 0, sizeof(RenderBuffer));


 glGenFramebuffersEXT(1, &rb->frame); 
 glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rb->frame);

 if(flags & 0x01){
  glGenTextures(1, &rb->color);
  glBindTexture(GL_TEXTURE_2D, rb->color);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, NULL);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, rb->color, 0);
 }
 if(flags & 0x02){
  glGenTextures(1, &rb->depth);
  glBindTexture(GL_TEXTURE_2D, rb->depth);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_INT, NULL);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

//  glDrawBuffer(GL_NONE);
//  glReadBuffer(GL_NONE);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, rb->depth, 0);
 }

 stat = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
 if(stat != GL_FRAMEBUFFER_COMPLETE_EXT){
  framebuffer_status(stat);
 }
 glBindTexture(GL_TEXTURE_2D, 0);
 glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

 return rb;
}

void kgmOGL::gcFreeRenderTarget(void* rb){
 if(!rb)
	 return;
 if( ((RenderBuffer*)rb)->color )
  glDeleteTextures(1, &((RenderBuffer*)rb)->color);
 if( ((RenderBuffer*)rb)->depth )
  glDeleteTextures(1, &((RenderBuffer*)rb)->depth);
 if( ((RenderBuffer*)rb)->frame )
  glDeleteFramebuffersEXT(1, &((RenderBuffer*)rb)->frame);
 delete (RenderBuffer*)rb;
}

void kgmOGL::gcSetRenderTarget(void* rb){
 if(!rb || !((RenderBuffer*)rb)->frame){
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  return;
 }

 glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ((RenderBuffer*)rb)->frame);
 GLenum err = glGetError();
 if(err != GL_NO_ERROR){
  int k = 0;
 }
 GLint ipar = 0;
 glEnable(GL_DEPTH_TEST);
 glDepthMask(true);
 glDepthFunc(GL_LEQUAL);
 glGetIntegerv(GL_DEPTH_BITS, &ipar);
 if(ipar == 0){
  int k = 0;
 }
//  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
//  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
//  glDrawBuffer(GL_BACK);
//  glReadBuffer(GL_BACK);
}

void kgmOGL::gcSetRenderTexture(void* rb, u32 stage, u32 flags){
 if(!rb || !((RenderBuffer*)rb)->frame){
  gcSetTexture(stage, 0);
  return;
 }
  
 if(flags & 0x01){
  glActiveTextureARB(GL_TEXTURE0_ARB + stage);
  glBindTexture(GL_TEXTURE_2D, ((RenderBuffer*)rb)->color);
 }
 if(flags & 0x02){
  glActiveTextureARB(GL_TEXTURE0_ARB + stage);
  glBindTexture(GL_TEXTURE_2D, ((RenderBuffer*)rb)->depth);
 }
}
*/
/*
void kgmOGL::gcCoordTexture(u32 coord, u32 mode, void* par){
 static GLenum   gl_coord = 0;
 static GLenum   gl_mode = 0;
 static GLenum	 gl_gen = 0;
 GLint    gl_ipar;
 GLfloat* gl_fpar;
 switch(coord){
 case gctex_s:
	 gl_coord = GL_S;
	 gl_gen = GL_TEXTURE_GEN_S;
	 break;
 case gctex_t:
	 gl_coord = GL_T;
	 gl_gen = GL_TEXTURE_GEN_T;
	 break;
 case gctex_r:
	 gl_coord = GL_R;
	 gl_gen = GL_TEXTURE_GEN_R;
	 break;
 case gctex_q:
	 gl_coord = GL_Q;
	 gl_gen = GL_TEXTURE_GEN_Q;
	 break;
 default:
	 gl_coord = GL_S;
	 gl_gen = GL_TEXTURE_GEN_S;
 }
 switch(mode){
 case gctex_gen:
	 gl_mode = GL_TEXTURE_GEN_MODE;
	 break;
 case gctex_obj:
	 gl_mode = GL_OBJECT_PLANE;
	 break;
 case gctex_eye:
	 gl_mode = GL_EYE_PLANE;
	 break;
 }

 if(mode == 0){
  glDisable(gl_gen);
  return;
 }else{
  glEnable(gl_gen);
 }

 if(gl_mode == GL_TEXTURE_GEN_MODE){
  switch((u32)par){
  case gctex_sphere:
	  gl_ipar = GL_SPHERE_MAP;
	  break;
  case gctex_obj:
	  gl_ipar = GL_OBJECT_LINEAR;
	  break;
  case gctex_eye:
	  gl_ipar = GL_EYE_LINEAR;
	  break;
  default:
	  gl_ipar = GL_SPHERE_MAP;
  }
  glTexGeni(gl_coord, gl_mode, gl_ipar);
 }else{
  gl_fpar = (GLfloat*)par;
  glTexGenfv(gl_coord, gl_mode, gl_fpar);
 }
}
*/
//CLIP PLANE
void kgmOGL::gcClipPlane(bool en, u32 id, float* par){
 GLdouble c[4] = {par[0], par[1], par[2], par[3]};
 glClipPlane(GL_CLIP_PLANE0 + id, c);
}
//STENCIL
void gcStencil(bool en, u32 func, u32 mask, u32 ref, 
			   u32 fail, u32 zfail, u32 zpass){

}
//Drawing
void kgmOGL::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt,
						  u32 i_size, u32 i_cnt, void *i_pnt){
 if(!v_pnt) return;

 unsigned char *pM = (unsigned char*)v_pnt;
 unsigned int  p_size  = sizeof(float) * 3;
 unsigned int  n_size  = sizeof(float) * 3;
 unsigned int  c_size  = sizeof(unsigned );
 unsigned int  uv_size = sizeof(float) * 2;

 if(v_fmt & gcv_uv_f4)
	 uv_size = sizeof(float) * 4;

 if(v_fmt & gcv_xyz){
  glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, v_size, pM);
  pM += (sizeof(float) * 3);
 }
 if(v_fmt & gcv_nor){
  glEnableClientState(GL_NORMAL_ARRAY);
   glNormalPointer(GL_FLOAT,v_size,pM);
  pM += (sizeof(float)*3);  
 }
 if(v_fmt & gcv_col){
  glEnableClientState(GL_COLOR_ARRAY);
   glColorPointer(4,GL_UNSIGNED_BYTE,v_size,pM);
  pM += sizeof(u32);  
 }
// if(v_fmt & gcv_spc){
//  glEnableClientState(GL_COLOR_ARRAY);
//   glColorPointer(4,GL_UNSIGNED_BYTE,v_size,pM);
//  pM += sizeof(u32);  
// }
 if(v_fmt & gcv_uv0){
  glClientActiveTextureARB(GL_TEXTURE0_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
  pM += (uv_size);
 }
 if(v_fmt & gcv_uv1){
  glClientActiveTextureARB(GL_TEXTURE1_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
  pM += (uv_size);
 }
 if(v_fmt & gcv_uv2){
  glClientActiveTextureARB(GL_TEXTURE2_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
  pM += (uv_size);
 }
 if(v_fmt & gcv_uv3){
  glClientActiveTextureARB(GL_TEXTURE3_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
  pM += (uv_size);
 }
 if(v_fmt & gcv_uv4){
  glClientActiveTextureARB(GL_TEXTURE4_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
  pM += (uv_size);
 }
 if(v_fmt & gcv_uv5){
  glClientActiveTextureARB(GL_TEXTURE5_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
  pM += (uv_size);
 }

 glColor3f(1, 1, 1);
 if(i_pnt && i_cnt){
  switch(i_size){
  case 4:
//   glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_INT, i_pnt);
   glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_INT, i_pnt);
   break;
  default:
//   glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_SHORT,i_pnt);
   glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_SHORT, i_pnt);
  }
 }else{
  glDrawArrays(gl_enum(pmt), 0, v_cnt);
 }

 glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void kgmOGL::gcDrawRect(int x, int y, int w, int h, u32 col){
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;
  V v[4];

  gc2DMode();
/*
  v[0].pos = vec3(rect.min.x, rect.min.y, 0); v[0].col = col; v[0].uv = vec2(0, 1);  
  v[1].pos = vec3(rect.min.x, rect.max.y, 0); v[1].col = col; v[1].uv = vec2(0, 1);  
  v[2].pos = vec3(rect.max.x, rect.max.y, 0); v[2].col = col; v[2].uv = vec2(1, 0);  
  v[3].pos = vec3(rect.max.x, rect.min.y, 0); v[3].col = col; v[3].uv = vec2(1, 1);  

  gcDraw(gcpmt_trianglefan, gcv_xyz | gcv_dif | gcv_tex, sizeof(V), 4, v, 0, 0);
*/
  v[0].pos = vec3(x,     y, 0);     v[0].col = col; v[0].uv = vec2(0, 0);
  v[1].pos = vec3(x,     y + h, 0); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(x + w, y, 0);     v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(x + w, y + h, 0); v[3].col = col; v[3].uv = vec2(1, 1);

  gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
  gc3DMode();
}

/*
void kgmOGL::gcDrawText(lRect clip, float w, float h, u32 col, char *text, u32 tlen){
  typedef struct{ vec3 pos; u32 col; vec2 uv; } V;
  V v[4];

  if(!text || !tlen || !m_font || !m_font->m_texture) 
   return;

  float tx = 0.0f, ty = 0.0f;
  float tdx = (float)(1.f / m_font->m_cols),
	    tdy = (float)(1.f / m_font->m_rows);

  float cx = (float)clip.min.x, cy = (float)clip.min.y;

  gc2DMode();

//  gcBlend(true, gcblend_one, gcblend_one);
  gcAlpha(true, gccmp_great, 0.5f);
  gcSetTexture(0, m_font);


  for(u32 i = 0; i < tlen; i++){
   char ch = text[i];
  
   if(ch == '\n'){ cx = (float)clip.min.x, cy += h; continue; }
   if(ch == ' ')  tx = 0.0f, ty = 0.0f;

   tx = (float)(tdx * (ch % m_font->m_rows));
   ty = 1.0f - (float)(tdy * (ch / m_font->m_rows));

   if(clip.InRect(long2(cx+w, cy+h)))
   {
    v[0].pos = vec3(cx, cy, 0),    v[0].col = col, v[0].uv = vec2(tx, ty);  
    v[1].pos = vec3(cx, cy+h, 0),  v[1].col = col, v[1].uv = vec2(tx, ty-tdy);  
    v[2].pos = vec3(cx+w, cy, 0),  v[2].col = col, v[2].uv = vec2(tx+tdx, ty);  
    v[3].pos = vec3(cx+w, cy+h, 0),v[3].col = col, v[3].uv = vec2(tx+tdx, ty-tdy);  
    gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
   }

   cx += w;
  }

//  gcBlend(0, 0, 0);
  gcAlpha(false, 0, 0);
  gcSetTexture(0, 0);
  gc3DMode();
}
*/

void  kgmOGL::gcDrawBillboard(vec3 pos, float w, float h, u32 col){
  mtx4 mv, mp, m; 
  vec3 rv, uv;
  typedef struct{ vec3 pos; u32 dif; vec2 uv; } V;
//  typedef struct{ vec3 pos; vec2 uv; } V;

  V v[4];

  gcGetMatrix(gcmtx_view, mv.m);
  gcGetMatrix(gcmtx_proj, mp.m);
  m = mv * mp;

  rv = vec3(m.m[0], m.m[4], m.m[8]); rv.normalize();
   rv.x *= w * 0.5f;
    rv.y *= w * 0.5f;
     rv.z *= w * 0.5f;
  uv = vec3(m.m[1], m.m[5], m.m[9]); uv.normalize();
   uv.x *= h * 0.5f;
    uv.y *= h * 0.5f;
     uv.z *= h * 0.5f;

  v[0].pos = pos - rv + uv;
   v[0].dif = col;
    v[0].uv.x = 0.0f, v[0].uv.y = 0.0f;
  v[1].pos = pos - rv - uv;
   v[1].dif = col;
    v[1].uv.x = 0.0f, v[1].uv.y = 1.0f;
  v[2].pos = pos + rv + uv;
   v[2].dif = col;
    v[2].uv.x = 1.0f, v[2].uv.y = 0.0f;
  v[3].pos = pos + rv - uv;
   v[3].dif = col;
    v[3].uv.x = 1.0f, v[3].uv.y = 1.0f;

  m.identity();
  gcSetMatrix(gcmtx_view, m.m);
  gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
//  glColor4f(1, 1, 1, 1);
}

//VERTEX & INDEX BUFFER 
void* kgmOGL::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize){
 VBO* vbo = new VBO;
 memset(vbo, 0, sizeof(VBO));

 if(vdata && vsize){
  glGenBuffersARB(1, &vbo->vb);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo->vb);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, vsize, vdata, GL_STATIC_DRAW_ARB);
 }
 if(idata && isize){
  glGenBuffersARB(1, &vbo->ib);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo->ib);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, isize, idata, GL_STATIC_DRAW_ARB);
 }
 glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
 glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
 return (void*)vbo;
}
void  kgmOGL::gcFreeVertexBuffer(void* b){
 VBO* vbo = (VBO*)b;
 if(!vbo)
	 return;
 if(vbo->vb)
  glDeleteBuffersARB(1, &vbo->vb);
 if(vbo->ib)
  glDeleteBuffersARB(1, &vbo->ib);
 delete vbo;
}
#define BUFFER_OFFSET(i) ((char*)NULL + (i))
void  kgmOGL::gcDrawVertexBuffer(void* b, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt,
									   u32 isize, u32 icnt, u32 ioff){
 VBO* vbo = (VBO*)b;
 u32 offset = 0;
 u32 uvt = (vfmt & gcv_uv_f4)?(4):(2);

 if(!vbo)
	 return;

 glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
 if(vbo->vb){
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo->vb);
 }
 if(vfmt & gcv_xyz){
  glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, vsize, (void*)offset); 
    offset = offset + sizeof(float) * 3;
 }
 if(vfmt & gcv_nor){
  glEnableClientState(GL_NORMAL_ARRAY);
   glNormalPointer(GL_FLOAT, vsize, (void*)offset);
    offset = offset + sizeof(float) * 3;
 }
 if(vfmt & gcv_col){
  glEnableClientState(GL_COLOR_ARRAY);
   glColorPointer(4,GL_UNSIGNED_BYTE, vsize, (void*)offset);
    offset = offset + sizeof(u32);
 }
// if(vfmt & gcv_spc){
//  glEnableClientState(GL_COLOR_ARRAY);
//   glColorPointer(4,GL_UNSIGNED_BYTE, vsize, (void*)offset);
//    offset = offset + sizeof(u32);
// }
 if(vfmt & gcv_uv0){
  glClientActiveTextureARB(GL_TEXTURE0_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
     offset = offset + sizeof(float) * uvt;
 }
 if(vfmt & gcv_uv1){
  glClientActiveTextureARB(GL_TEXTURE1_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
     offset = offset + sizeof(float) * uvt;
 }
 if(vfmt & gcv_uv2){
  glClientActiveTextureARB(GL_TEXTURE2_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
     offset = offset + sizeof(float) * uvt;
 }
 if(vfmt & gcv_uv3){
  glClientActiveTextureARB(GL_TEXTURE3_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
     offset = offset + sizeof(float) * uvt;
 }
 if(vfmt & gcv_uv4){
  glClientActiveTextureARB(GL_TEXTURE4_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
     offset = offset + sizeof(float) * uvt;
 }
 if(vfmt & gcv_uv5){
  glClientActiveTextureARB(GL_TEXTURE5_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
     offset = offset + sizeof(float) * uvt;
 }
 if(vfmt & gcv_uv6){
  glClientActiveTextureARB(GL_TEXTURE6_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
     offset = offset + sizeof(float) * uvt;
 }
 if(vfmt & gcv_uv7){
  glClientActiveTextureARB(GL_TEXTURE7_ARB);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
     offset = offset + sizeof(float) * uvt;
 }

// glColor3f(1, 1, 1);
 if(vbo->ib && icnt){
//  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo->ib);
  switch(isize){
  case 4:
   glIndexPointer(GL_UNSIGNED_INT, 0, (void*)ioff);
//   glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_INT, (void*)ioff);
   glDrawRangeElements (gl_enum(pmt), 0, vcnt - 1, icnt, GL_UNSIGNED_INT, ((char*)0L + ioff));
   break;
  default:
//   glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);
   glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_SHORT, BUFFER_OFFSET(ioff));
//   glDrawRangeElements (gl_enum(pmt), ioff, ioff + icnt, icnt/3, GL_UNSIGNED_SHORT, (void*)0);
//   glDrawRangeElements (gl_enum(pmt), 0, vcnt - 1, icnt, GL_UNSIGNED_INT, (void*)ioff);//(ioff/1000));
  }
 }
 else{
  glDrawArrays(gl_enum(pmt), 0, vcnt);
 }

 glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
 glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
 glDisableClientState(GL_VERTEX_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
 glDisableClientState(GL_COLOR_ARRAY);
 glDisableClientState(GL_TEXTURE_COORD_ARRAY);

 glPopClientAttrib();
}

// SHADERS
GLint v_shad;
void* kgmOGL::gcGenShader(const char* vsrc, const char* fsrc){
 GLhandleARB prog = 0;
 GLhandleARB vshad = 0, fshad = 0;
 int stat[1] = {0};
 int size = 256;
 char tbuf[256];

 prog = glCreateProgramObjectARB();

//GL_VERTEX_SHADER_ARB
 if(vsrc){
  size = strlen(vsrc);
  v_shad = vshad = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  glShaderSourceARB(vshad, 1, (const GLcharARB**)&vsrc, &size);
  glCompileShaderARB(vshad);
  glGetObjectParameterivARB(vshad, GL_OBJECT_COMPILE_STATUS_ARB, stat);

  if(stat[0] == GL_FALSE){
   glGetInfoLogARB(vshad, 256, &size, tbuf);
#ifdef WIN32
   MessageBox(0, tbuf, "VShader", 0);
#elif defined LINUX
   printf("VShader: %s", tbuf); 
#endif
  }
  glAttachObjectARB(prog, vshad);
//  glDeleteObjectARB(vshad);
 }

 if(fsrc){
  size = strlen(fsrc);
  fshad = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
  glShaderSourceARB(fshad, 1, (const GLcharARB**)&fsrc, NULL);
  glCompileShaderARB(fshad);
  glAttachObjectARB(prog, fshad);
  glGetObjectParameterivARB(fshad, GL_OBJECT_COMPILE_STATUS_ARB, stat);

  if(stat[0] == GL_FALSE){
   glGetInfoLogARB(fshad, 256, &size, tbuf);
#ifdef WIN32
   MessageBox(0, tbuf, "FShader", 0);
#elif defined LINUX
   printf("FShader: %s", tbuf); 
#endif
  }
  glAttachObjectARB(prog, fshad);
//  glDeleteObjectARB(fshad);
 }

 glLinkProgramARB(prog);
 glGetObjectParameterivARB(prog, GL_OBJECT_LINK_STATUS_ARB, stat);
 if(stat[0] == GL_FALSE){
  glGetInfoLogARB(prog, 256, &size, tbuf);
  printf("LogARB: %s", tbuf);
 }
// glUseProgramARB();
 return (void*)prog;
}
void kgmOGL::gcFreeShader(void* s){
// glDetachObjectARB(((ShadeStruct*)shad), ((ShadeStruct*)shad));
 if(s){
  glDeleteObjectARB((GLhandleARB)s);
 }
}
void kgmOGL::gcSetShader(void* s){
 if(s){
  glUseProgramObjectARB((GLhandleARB)s);
  GLenum err = glGetError();
  if(err != GL_NO_ERROR){
    const GLubyte* ers = gluErrorString(err);
    printf("some error fixed :(  %s\n", ers);
  }
 }
 else{
  glUseProgramObjectARB(0);
 }
}

void kgmOGL::gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val)
{ 
 GLint link = glGetUniformLocationARB((GLhandleARB)s, par);
 if(link < 0)
	 return;

 if(type == 0){
  switch(cnt){
  case 1:
     glUniform1fvARB(link, 1, (float*)val);
	 break;
  case 2:
     glUniform2fvARB(link, 1, (float*)val);
	 break;
  case 3:
     glUniform3fvARB(link, 1, (float*)val);
	 break;
  case 4:
     glUniform4fvARB(link, 1, (float*)val);
	 break;
  }
 }else if(type == 1){
  switch(cnt){
  case 1:
     glUniform1ivARB(link, 1, (const int*)val);
	 break;
  case 2:
     glUniform2ivARB(link, 1, (const int*)val);
	 break;
  case 3:
     glUniform3ivARB(link, 1, (const int*)val);
	 break;
  case 4:
     glUniform4ivARB(link, 1, (const int*)val);
	 break;
  }
 }
}

void kgmOGL::gcUniformMatrix(void* s, u32 cnt, u32 trn, const char* par, void* val)
{ 
 GLint link = glGetUniformLocationARB((GLhandleARB)s, par);
 if(link < 0)
	 return;
 switch(cnt){
 case 2:
     glUniformMatrix2fvARB(link, 1, (GLboolean)trn, (float*)val);
	 break;
 case 3:
     glUniformMatrix3fvARB(link, 1, (GLboolean)trn, (float*)val);
	 break;
 case 4:
     glUniformMatrix4fvARB(link, 1, (GLboolean)trn, (float*)val);
	 break;
 }
}

void kgmOGL::gcUniformSampler(void* s, const char* par, void* val){
 GLint link = glGetUniformLocationARB((GLhandleARB)s, par);
 if(link < 0)
	 return;
// glUniform1ivARB(link, 1, (const int*)val);
 glUniform1iARB(link, (GLu32)val);
}

