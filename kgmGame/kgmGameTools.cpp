#include "kgmGameTools.h"
#include "kgmIGame.h"

#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmMemory.h"
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmMath/kgmRect2d.h"
#include "../kgmMedia/kgmSound.h"
#include "../kgmGraphics/kgmPicture.h"
#include "../kgmGraphics/kgmGraphics.h"
#include "../kgmGraphics/kgmParticles.h"

#include "kgmActor.h"

static char str_buf[1024];

kgmGameTools::kgmGameTools()
{
}

kgmGameTools::~kgmGameTools()
{
}

//*************** DRAWING ***************
/*
void kgmGameTools::gcDrawRect(kgmIGC* gc, int x, int y, int w, int h, u32 col, void* tex){
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;
  V v[4];

  v[0].pos = vec3(x,     y,     0); v[0].col = col; v[0].uv = vec2(0, 0);
  v[1].pos = vec3(x,     y + h, 0); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(x + w, y,     0); v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(x + w, y + h, 0); v[3].col = col; v[3].uv = vec2(1, 1);

  if(tex)
    gc->gcSetTexture(0, tex);
  gc->gc2DMode();
  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
  gc->gc3DMode();
  if(tex)
    gc->gcSetTexture(0, 0);
}

void kgmGameTools::gcDrawText(kgmIGC* gc, kgmFont* font, int fw, int fh, int x, int y, int w, int h, u32 col, kgmString& text){
  typedef struct{ vec3 pos; u32 col; vec2 uv; } V;
  V v[4];
  kgmRect2d<int> clip(x, y, x + w, y + h);

  if(text.length() < 1)
    return;

  if(!font || !font->m_texture)
    return;

  u32 tlen = text.length();
  float tx = 0.0f, ty = 0.0f;
  float tdx = (float)(1.f / font->m_cols),
  tdy = (float)(1.f / font->m_rows);

  float cx = (float)x, cy = (float)y;

  gc->gc2DMode();
  gc->gcAlpha(true, gccmp_great, 0.5f);
  gc->gcSetTexture(0, font->m_texture);


  for(u32 i = 0; i < tlen; i++){
    char ch = text[i];

    if(ch == '\n'){
      cx = (float)x;
      cy += fh;
      continue;
    }
    if((ch == ' ') || ((ch == '\t')))
      tx = 0.0f, ty = 0.0f;

    tx = (float)(tdx * (ch % font->m_rows));
    ty = 1.0f - (float)(tdy * (ch / font->m_rows));

    if(clip.inside(cx + fw, cy + fh))
    {
      v[0].pos = vec3(cx, cy, 0),    v[0].col = col, v[0].uv = vec2(tx, ty);
      v[1].pos = vec3(cx, cy+fh, 0),  v[1].col = col, v[1].uv = vec2(tx, ty-tdy);
      v[2].pos = vec3(cx+fw, cy, 0),  v[2].col = col, v[2].uv = vec2(tx+tdx, ty);
      v[3].pos = vec3(cx+fw, cy+fh, 0),v[3].col = col, v[3].uv = vec2(tx+tdx, ty-tdy);
      gc->gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
    }

    cx += fw;
  }

  gc->gcAlpha(false, 0, 0.0);
  gc->gcSetTexture(0, 0);
  gc->gc3DMode();
}
*/

kgmPicture* kgmGameTools::genPicture(kgmMemory<u8>& m)
{
  u32 id_tga = 0x00020000;

  if(m.empty() || !m.data())
    return 0;

  if(!memcmp("BM", m.data(), 2))
    return genPictureFromBmp(m);

  if(!memcmp(&id_tga, m.data(), 4))
    return genPictureFromTga(m);

  return 0;
}

kgmPicture* kgmGameTools::genPictureFromBmp(kgmMemory<u8>& m)
{
  if(m.empty())
    return 0;

  u16 f_type;
  u32   f_size;
  u16 f_res0,f_res1;
  u32   f_boffs;
  u32   b_size;
  long   b_width,b_height;
  u16 b_planes,b_btcnt;
  u32   b_compr,b_sizeimg;
  long   b_xppm,b_yppm;
  u32   b_clus,b_climp;
  char*  pm = (char*)m.data();

  int   width = 0;
  int   height = 0;
  int   bpp = 0;
  int   frames = 0;
  void* pdata = 0;

  memcpy(&f_type, pm, 2);	pm += 2;
  memcpy(&f_size, pm, 4);	pm += 4;
  memcpy(&f_res0, pm, 2);	pm += 2;
  memcpy(&f_res1, pm, 2);	pm += 2;
  memcpy(&f_boffs, pm, 4);	pm += 4;
  memcpy(&b_size, pm, 4);	pm += 4;
  memcpy(&b_width, pm, 4);	pm += 4;
  memcpy(&b_height, pm, 4);	pm += 4;
  memcpy(&b_planes, pm, 2);	pm += 2;
  memcpy(&b_btcnt, pm, 2);	pm += 2;
  memcpy(&b_compr, pm, 4);	pm += 4;
  memcpy(&b_sizeimg, pm, 4);	pm += 4;
  memcpy(&b_xppm, pm, 4);	pm += 4;
  memcpy(&b_yppm, pm, 4);	pm += 4;
  memcpy(&b_clus, pm, 4);	pm += 4;
  memcpy(&b_climp, pm, 4);  pm += 4;

  if(memcmp("BM",&f_type,2))
    return 0;

  if(!b_width || !b_height)
    return 0;

  if(b_btcnt < 8)
    return 0;

  if(b_btcnt == 8)
  {
    u32 r_size = b_width * b_height;
    pdata = (u32*)malloc(sizeof(u32) * r_size);
    u32 *pal = (u32*)malloc(sizeof(u32) * 256);
    memcpy(pal, pm, 256 * 4);	pm += 256 * 4;

    for(u32 i = 0; i < r_size; i++)
    {
      uchar ind = 0;
      memcpy(&ind, pm, 1);	pm += 1;
      ((u32*)pdata)[i] = pal[ind];
    }

    width = b_width;
    height = b_height;
    bpp = 32;
    free(pal);

    return new kgmPicture(width, height, bpp, frames, pdata);
  }

  u32 r_size = b_width * b_height * (b_btcnt / 8);
  pdata = malloc(sizeof(char) * r_size);
  memcpy(pdata, pm, r_size);
  width = b_width;
  height = b_height;
  bpp = b_btcnt;

  for(int i = 0; i < (width * height); i++)
  {
    char* pt = (char*)(((char*)pdata) + i * b_btcnt / 8);
    char  t  = pt[0];
    pt[0] = pt[2];
    pt[2] = t;
  }

  return new kgmPicture(width, height, bpp, frames, pdata);
}

kgmPicture* kgmGameTools::genPictureFromTga(kgmMemory<u8>& m)
{
  if(m.empty())
    return 0;

  uchar idl, cmp, dt, btcnt, dsc;
  uchar clmap[5];
  u16   xorgn, yorgn, w, h;
  char* pm = (char*)m.data();

  int   width = 0;
  int   height = 0;
  int   bpp = 0;
  int   frames = 0;
  void* pdata = 0;

  memcpy(&idl,   pm, 1);  pm += 1;
  memcpy(&cmp,   pm, 1);  pm += 1;
  memcpy(&dt,    pm, 1);  pm += 1;
  memcpy(&clmap, pm, 5);  pm += 5;
  memcpy(&xorgn, pm, 2);  pm += 2;
  memcpy(&yorgn, pm, 2);  pm += 2;
  memcpy(&w,     pm, 2);  pm += 2;
  memcpy(&h,     pm, 2);  pm += 2;
  memcpy(&btcnt, pm, 1);  pm += 1;
  memcpy(&dsc,   pm, 1);  pm += 1;

  if(dt != 2)
    return 0;

  if((btcnt != 24) && (btcnt != 32))
    return 0;

  u32 bt_pp = btcnt / 8;
  u32 r_size = w * h * bt_pp;
  pm += idl;
  pdata = malloc(sizeof(char) * r_size);
  memcpy(pdata, pm, r_size);
  u32 k = (u32)((u32*)pdata)[0];
  width = w;
  height = h;
  bpp = btcnt;

  u32 order = (dsc & 0x20);

  if(order)
  {
    for(int i = (w * h); i > 0; i--)
    {
      char* pt = (char*)(((char*)pdata) + (i - 1) * bt_pp);
      char  t  = pt[0];

      pt[0] = pt[2];
      pt[2] = t;
    }
  }
  else
  {
    for(int i = 0; i < (w * h); i++)
    {
      char* pt = (char*)(((char*)pdata) + i * bt_pp);
      char  t  = pt[0];

      pt[0] = pt[2];
      pt[2] = t;
    }
  }

  return new kgmPicture(width, height, bpp, frames, pdata);
}

kgmTexture* kgmGameTools::genTexture(kgmIGC* gc, kgmMemory<u8>& m)
{
  kgmPicture* pic = genPicture(m);

  if(!pic)
  {
    return 0;
  }
  else
  {
    int nwidth, nheight;

    switch(kgmGraphics::textureQuality)
    {
    case kgmGraphics::GraphicsQualityHight:
      nwidth = (256 >= (pic->width / 2)) ? (pic->width / 2) : (pic->width);
      nheight = (256 >= (pic->height / 2)) ? (pic->height / 2) : (pic->height);
      pic->resample(nwidth, nheight);
      break;
    case kgmGraphics::GraphicsQualityMedium:
      nwidth = (128 >= (pic->width / 4)) ? (pic->width / 4) : (pic->width);
      nheight = (128 >= (pic->height / 4)) ? (pic->height / 4) : (pic->height);
      pic->resample(nwidth, nheight);
      break;
    case kgmGraphics::GraphicsQualityLow:
      nwidth = (64 >= (pic->width / 8)) ? (pic->width / 8) : (pic->width);
      nheight = (64 >= (pic->height / 8)) ? (pic->height / 8) : (pic->height);
      pic->resample(nwidth, nheight);
      break;
    }
  }

  u32 fmt = gctex_fmt32;

  switch(pic->bpp)
  {
  case 8:
    fmt = gctex_fmt8;
    break;
  case 16:
    fmt = gctex_fmt16;
    break;
  case 24:
    fmt = gctex_fmt24;
    break;
  case 32:
    fmt = gctex_fmt32;
    break;
  };

  kgmTexture* tex = new kgmTexture();

  tex->m_texture = gc->gcGenTexture(pic->pdata, pic->width, pic->height, fmt, gctype_tex2d);
  pic->release();

  if(!tex->m_texture)
  {
    tex->release();

    return 0;
  }

  return tex;
}

kgmFont* kgmGameTools::genFont(kgmIGC* gc, u32 w, u32 h, u32 r, u32 c, kgmMemory<u8>& m)
{
  kgmPicture* pic = genPicture(m);

  if(!pic)
    return 0;

  u32 fmt = gctex_fmt32;

  switch(pic->bpp)
  {
  case 8:
    fmt = gctex_fmt8;
    break;
  case 16:
    fmt = gctex_fmt16;
    break;
  case 24:
    fmt = gctex_fmt24;
    break;
  case 32:
    fmt = gctex_fmt32;
    break;
  };

  kgmFont* font = new kgmFont();

  gc->gcSet(gctex_fltmag, (void*)gcflt_near);
  gc->gcSet(gctex_fltmin, (void*)gcflt_near);

  font->m_texture  = gc->gcGenTexture(pic->pdata, pic->width, pic->height, fmt, gctype_tex2d);

  gc->gcSet(gctex_fltmag, (void*)gcflt_linear);
  gc->gcSet(gctex_fltmin, (void*)gcflt_linear);

  pic->release();

  if(!font->m_texture)
  {
    font->release();

    return 0;
  }

  font->m_rows = r;
  font->m_cols = c;

  return font;
}

// SHADER
kgmShader* kgmGameTools::genShader(kgmIGC* gc, kgmString& s){
  char* mem_vsh = 0;
  char* mem_fsh = 0;

  if(!gc || s.empty())
    return 0;

  mem_fsh = (char*)strstr((char*)s, "//Fragment Shader");
  if(mem_fsh){
    size_t sh_s = (size_t)mem_fsh - (size_t)s.data();
    mem_vsh = (char*)malloc(sh_s + 1);
    memcpy(mem_vsh, s.data(), sh_s);
    mem_vsh[sh_s] = '\0';
    sh_s = s.length() - sh_s;
    char* fsh = mem_fsh;
    mem_fsh = (char*)malloc(sh_s + 1);
    memcpy(mem_fsh, fsh, sh_s);
    mem_fsh[sh_s] = '\0';
  }else{
    mem_vsh = (char*)malloc(s.length() + 1);
    memcpy(mem_vsh, s.data(), s.length());
    mem_vsh[s.length()] = '\0';
  }

  kgmShader* shader = new kgmShader(gc);
  shader->m_shader = gc->gcGenShader((const char*)mem_vsh, (const char*)mem_fsh);

  if(mem_vsh) free(mem_vsh);
  if(mem_fsh) free(mem_fsh);

  if(!shader->m_shader){
    shader->release();
    shader = 0;
  }

  return shader;
}

kgmShader* kgmGameTools::genShader(kgmIGC* gc, kgmXml& xml){
  char* mem_vsh = 0;
  char* mem_fsh = 0;

  if(!gc || !xml.m_node)
    return 0;

  if(xml.m_node->node(0) && xml.m_node->node(0)->m_name == "VertexShader")
    mem_vsh = xml.m_node->node(0)->m_data.data();
  else if(xml.m_node->node(0) && xml.m_node->node(0)->m_name == "FragmentShader")
    mem_fsh = xml.m_node->node(0)->m_data.data();

  if(xml.m_node->node(1) && xml.m_node->node(1)->m_name == "VertexShader")
    mem_vsh = xml.m_node->node(1)->m_data.data();
  else if(xml.m_node->node(1) && xml.m_node->node(1)->m_name == "FragmentShader")
    mem_fsh = xml.m_node->node(1)->m_data.data();

  kgmShader* shader = new kgmShader(gc);
  shader->m_shader = gc->gcGenShader((const char*)mem_vsh, (const char*)mem_fsh);

  if(!shader->m_shader){
    shader->release();
    shader = null;
  }

  return shader;
}

kgmMaterial* kgmGameTools::genMaterial(kgmMemory<u8>& m){
  kgmMaterial* mtl = 0;
  char* str = 0;
  char* key = 0;
  char* val = 0;
  char* p   = (char*)m.data();
  u32  i   = 0;
  float farr[4];

  if(m.empty())
    return 0;

  mtl = new kgmMaterial;
  str = new char[1024];
  key = new char[128];
  val = new char[128];

  while(!m.eof())
  {
    memset(str, 0, 1024);
    memset(key, 0, 128);
    memset(val, 0, 128);

    m.reads((u8*)str, 1024, (u8*)"\n", 1);
    sscanf(str, "%s %s", key, val);

    if(!strcmp(key, "color")){
      sscanf(str, "%s %f %f %f %f", key, &farr[0], &farr[1], &farr[2], &farr[3]);
      mtl->m_color = kgmMaterial::Color(farr[0], farr[1], farr[2], farr[3]);
    }
    if(!strcmp(key, "map_texture")){
      mtl->setTexColor(kgmIGame::getGame()->getResources()->getTexture(val));
    }
    if(!strcmp(key, "shader")){
      mtl->setShader(kgmShader::toType(val));
    }
  }

  delete str;
  delete key;
  delete val;

  return mtl;
}

kgmMaterial* kgmGameTools::genMaterial(kgmXml& x){
  kgmMaterial* mtl = 0;
  float        farr[4];

  kgmXml::Node* mnode = 0;

  if(!x.m_node || !(mnode = x.m_node->node("kgmMaterial")))
    return 0;

  mtl = new kgmMaterial;

  for(int i = 0; i < mnode->nodes(); i++){
    kgmString id, val;

    if(mnode->node(i))
      mnode->node(i)->id(id);
    else
      break;

    if(id == "Color"){
      mnode->node(i)->attribute("value", val);
      sscanf(val.data(), "%f %f %f ", &farr[0], &farr[1], &farr[2]);
      mtl->m_color = kgmMaterial::Color(farr[0], farr[1], farr[2], 1.0);
    }
    else if(id == "Alpha")
    {
      if(mnode->node(i)->attribute("value", val))
      {
        float a;

        sscanf(val, "%f", &a);
        mtl->m_transparency = 1.0f - a;

        if(a < 1.0f)
        {
          mtl->m_alpha = true;
        }
      }
    }
    else if(id == "Cull")
    {
      mtl->m_2side = true;
    }
    else if(id == "map_color")
    {
      mnode->node(i)->attribute("value", val);
      mtl->setTexColor(kgmIGame::getGame()->getResources()->getTexture(val));
    }
    else if(id == "map_normal")
    {
      mnode->node(i)->attribute("value", val);
      mtl->setTexNormal(kgmIGame::getGame()->getResources()->getTexture(val));
    }
    else if(id == "map_specular")
    {
      mnode->node(i)->attribute("value", val);
      mtl->setTexSpecular(kgmIGame::getGame()->getResources()->getTexture(val));
    }
    else if(id == "Shader")
    {
      mnode->node(i)->attribute("value", val);
      mtl->setShader(kgmShader::toType(val));
    }
  }

  return mtl;
}


//SKELETON
kgmSkeleton* kgmGameTools::genSkeleton(kgmMemory<u8>& m)
{
  kgmSkeleton* skel;
  char* str = 0;
  char* key = 0;
  char* val = 0;
  char* p   = (char*)m.data();
  u32  i   = 0;

  if(m.empty())
    return 0;

  skel = new kgmSkeleton;
  str = new char[1024];
  key = new char[128];
  val = new char[128];

  while(!m.eof()){
    memset(str, 0, 1024);
    memset(key, 0, 128);
    memset(val, 0, 128);

    if(0 == m.reads((u8*)str, 1024, (u8*)"\n", 1))
      break;

    sscanf(str, "%s %s", key, val);

    if(!strcmp(key, "Bone")){
      kgmSkeleton::Joint* joint = new kgmSkeleton::Joint;
      char* c = strstr(str, "Bone");

      if(c)
      {
        c += 5;
        strncpy(joint->n, c, sizeof(joint->n));
      }

      memset(str, 0, 1024);

      if(0 == m.reads((u8*)str, 1024, (u8*)"\n", 1))
        break;

      sscanf(str, "%i %f %f %f %f %f %f %f", &joint->i,
      &joint->v.x, &joint->v.y, &joint->v.z,
      &joint->r.x, &joint->r.y, &joint->r.z, &joint->r.w);
      skel->m_joints.add(joint);
    }
  }

  delete str;
  delete key;
  delete val;

  if(skel->m_joints.size() < 1){
    skel->release();
    skel = 0;
  }
  skel->update();
  return skel;
}

kgmSkeleton* kgmGameTools::genSkeleton(kgmXml& x){
  kgmSkeleton* skel;

  kgmXml::Node* snode = 0;
  if(!x.m_node || !(snode = x.m_node->node("kgmSkeleton")))
    return 0;

  skel = new kgmSkeleton;

  for(int i = 0; i < snode->nodes(); i++){
    kgmString id;
    snode->node(i)->id(id);
    if(id == "Bone"){
      kgmString name, par, pos, rot, eul;
      kgmSkeleton::Joint* joint = new kgmSkeleton::Joint;

      snode->node(i)->attribute("name", name);
      snode->node(i)->attribute("parent", par);
      snode->node(i)->attribute("position", pos);
      snode->node(i)->attribute("quaternion", rot);
      snode->node(i)->attribute("euler", eul);

      vec3 vrot;

      strcpy(joint->n, name.data());

      if(par == "None")
        joint->i = -1;
      else
        joint->i = skel->getJointIndex(par.data());

      sscanf(pos.data(), "%f %f %f", &joint->v.x, &joint->v.y, &joint->v.z);
      sscanf(rot.data(), "%f %f %f %f", &joint->r.x, &joint->r.y, &joint->r.z, &joint->r.w);
      sscanf(eul.data(), "%f %f %f", &vrot.x, &vrot.y, &vrot.z);

      //joint->r.euler(vrot);

      skel->m_joints.add(joint);
    }
  }


  if(skel->m_joints.size() < 1){
    skel->release();
    skel = 0;
  }

  skel->update();
  return skel;
}

//ANIMATION
kgmAnimation* kgmGameTools::genAnimation(kgmMemory<u8>& m){
  kgmAnimation* anim = 0;
  kgmAnimation::Animation*   f = 0;
  int tick = 0;
  char* str = 0;
  char* key = 0;

  if(m.empty())
    return anim;

  anim = new kgmAnimation;
  str = new char[1024];
  key = new char[512];

  while(!m.eof()){
    memset(str, 0, 1024);
    memset(key, 0, 512);

    if(0 == m.reads((u8*)str, 512, (u8*)"\n", 1))
      break;
    //   str_close(str);
    sscanf(str, "%s", key);
    //   continue;

    if(!strcmp(key, "FrameStart")){
      sscanf(str, "FrameStart %i", &anim->m_start);
      continue;
    }
    if(!strcmp(key, "FrameEnd")){
      sscanf(str, "FrameEnd %i",   &anim->m_end);
      continue;
    }
    if(!strcmp(key, "FPS")){
      sscanf(str, "FPS %i",   &anim->m_fps);
      anim->m_fps = 45.0;
      continue;
    }

    if(!strcmp(key, "Frames")){
      f = new kgmAnimation::Animation;
      f->m_name =  &str[7];
      anim->addNode(f);
      tick = 0;
    }
    if(!strcmp(key, "Key")){
      int time;
      vec3 pos;
      quat rot;
      sscanf(str, "%s %i %f %f %f %f %f %f %f", key, &time,
      &pos.x, &pos.y, &pos.z, &rot.x, &rot.y, &rot.z, &rot.w);
      if(tick <= time){
        f->addFrame(time, pos, rot);
        tick = time;
      }
    }
  }

  if(anim->count() < 1){
    anim->release();
    anim = 0;
  }

  delete [] str;
  delete [] key;
  return anim;
}

kgmAnimation* kgmGameTools::genAnimation(kgmXml& x){
  kgmAnimation* anim = 0;
  kgmAnimation::Animation* f = 0;
  kgmString id, val;
  int tick = 0;

  kgmXml::Node* anode = 0;

  if(!x.m_node || !(anode = x.m_node->node("kgmAnimation")))
    return null;

  anim = new kgmAnimation;

  anode->attribute("fstart", val);
  sscanf(val.data(), "%i", &anim->m_start);
  anode->attribute("fend", val);
  sscanf(val.data(), "%i", &anim->m_end);
  anode->attribute("fps", val);
  sscanf(val.data(), "%i", &anim->m_fps);


  for(int i = 0; i < anode->nodes(); i++){
    kgmString id, val;
    anode->node(i)->id(id);

    if(id == "Animation"){
      f = new kgmAnimation::Animation;
      anode->node(i)->attribute("name", val);
      f->m_name = val;
      anim->addNode(f);
      tick = 0;
      for(int j = 0; j < anode->node(i)->nodes(); j++){
        int  time;
        vec3 pos, eul;
        quat rot;
        kgmString id, tm, ps, rt, el;
        anode->node(i)->node(j)->id(id);
        if(id == "Frame"){
          anode->node(i)->node(j)->attribute("key", tm);
          anode->node(i)->node(j)->attribute("position", ps);
          anode->node(i)->node(j)->attribute("quaternion", rt);
          anode->node(i)->node(j)->attribute("euler", el);
          sscanf(tm.data(), "%i", &time);
          sscanf(ps.data(), "%f %f %f", &pos.x, &pos.y, &pos.z);
          sscanf(el.data(), "%f %f %f", &eul.x, &eul.y, &eul.z);
          sscanf(rt.data(), "%f %f %f %f", &rot.x, &rot.y, &rot.z, &rot.w);
          /*rot.euler(eul);
          eul.y += PI / 2;
          mtx4 mtx(rot);
          vec3 ang, axis;
          mtx.angles(ang);
          axis = vec3(-1, 0, 0);
          quat q1(axis, PI / 2);
          rot = rot + q1;
          rot.normalize();
          axis = vec3(0, 1, 0);
          q1 = quat(axis, -PI);
          rot = rot + q1;
          rot.normalize();
          rot.euler(eul);*/
          if(tick <= time){
            f->addFrame(time, pos, rot);
            tick = time;
          }
        }
      }
    }
  }

  if(anim->count() < 1){
    anim->release();
    anim = 0;
  }
  return anim;
}


//MESHES
kgmMesh* kgmGameTools::genMesh(kgmMemory<u8>& mm){
  kgmList<kgmMaterial*> materials;
  kgmMaterial* mtl = 0;

  kgmMesh* m = 0;
  char* key = 0;
  char* val = 0;
  char* str = 0;
  int   col[4] = {0};

  if(mm.empty())
    return null;

  key = new char[128];
  val = new char[128];
  str = new char[512];

  while(!mm.eof()){
    memset(key, 0, 128);
    memset(val, 0, 128);
    memset(str, 0, 512);
    mm.reads((u8*)str, 512, (u8*)"\n", 1);
    sscanf(str, "%s", key);

    if(!strcmp(key, "Mesh"))
    {
      sscanf(str, "%s %s", key, val);
      if(m)
        break;
      m = new kgmMesh();
      strcpy(m->m_id, val);
    }

    if(!strcmp(key, "Vertices"))
    {
      u32 count = 0;
      u32 maps  = 0;
      kgmMesh::Vertex_P_N_C_T2*  v = 0;
      sscanf(str, "%s %i %i", key, &count, &maps);
      v = (kgmMesh::Vertex_P_N_C_T2*)m->vAlloc(count, kgmMesh::FVF_P_N_C_T2);

      for(int i = 0; i < count; i++)
      {
        mm.reads((u8*)str, 512, (u8*)"\n", 1);
        sscanf(str, "%f %f %f %f %f %f %f %f",
        &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
        &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
        &v[i].uv[0].x, &v[i].uv[0].y);
        v[i].col = 0xffffffff;
      }
    }
    if(!strcmp(key, "Faces")){
      u32 count = 0;
      kgmMesh::Face_16*   f = 0;
      kgmArray<u16> fmaps;
      sscanf(str, "%s %i", key, &count);
      f = (kgmMesh::Face_16*)m->fAlloc(count);
      fmaps.realloc(count);

      for(int i = 0; i < count; i++)
      {
        u32 fi[3];
        mm.reads((u8*)str, 512, (u8*)"\n", 1);
        sscanf(str, "%i %i %i %i", &fi[0], &fi[1], &fi[2]);
        f[i].a = fi[0];
        f[i].b = fi[1];
        f[i].c = fi[2];
      }
    }
    if(!strcmp(key, "Skin")){
      u32 count = 0;
      sscanf(str, "%s %i", key, &count);
      kgmMesh::Vertex_P_N_C_T2* v = new kgmMesh::Vertex_P_N_C_T2[ m->vcount() ];
      memcpy(v, m->vertices(), m->vcount() * m->vsize());
      kgmMesh::Vertex_P_N_C_T_BW_BI* s = (kgmMesh::Vertex_P_N_C_T_BW_BI*)m->vAlloc(count,kgmMesh::FVF_P_N_C_T_BW_BI);

      for(int i = 0; i < count; i++)
      {
        u32  nw = 0;
        u32  bi[4] = {0};
        float bw[4] = {0.0f};
        mm.reads((u8*)str, 512, (u8*)"\n", 1);
        sscanf(str, "%i %i %i %i %i %f %f %f %f", &nw,
        &bi[0], &bi[1], &bi[2], &bi[3],
        &bw[0], &bw[1], &bw[2], &bw[3]);
        s[i].bw[0] = bw[0];
        s[i].bi[0] = bi[0];
        s[i].bw[1] = bw[1];
        s[i].bi[1] = bi[1];
        s[i].bw[2] = bw[2];
        s[i].bi[2] = bi[2];
        s[i].bw[3] = bw[3];
        s[i].bi[4] = bi[3];
        s[i].pos = v[i].pos;
        s[i].nor = v[i].nor;
        s[i].col = v[i].col;
        s[i].uv = vec2(v[i].uv[0].x, v[i].uv[0].y);
      }

      delete [] v;
    }
  }

  for(int i = 0; i < materials.size(); i++)
    materials[i]->release();

  materials.clear();

  delete [] key;
  delete [] val;
  delete [] str;

  return m;
}

kgmMesh* kgmGameTools::genMesh(kgmXml& x){
  kgmList<kgmMaterial*> materials;
  kgmMaterial* mtl = 0;

  kgmMesh* m = 0;
  int   col[4] = {0};

  kgmXml::Node* mnode = 0;

  if(!x.m_node || !(mnode = x.m_node->node("kgmMesh")))
    return null;

  m = new kgmMesh();
  mnode->attribute("id", m->m_id);

  for(int i = 0; i < mnode->nodes(); i++)
  {
    kgmString id, val, data;

    if(mnode->node(i))
      mnode->node(i)->id(id);
    else
      break;

    if(id == "Vertices")
    {
      u32 count = 0;
      kgmMesh::Vertex_P_N_C_T*  v = 0;

      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      v = (kgmMesh::Vertex_P_N_C_T*)m->vAlloc(count, kgmMesh::FVF_P_N_C_T);
      mnode->node(i)->data(data);

      char* p = data.data();

      for(int i = 0; i < count; i++)
      {
        u32 rd;

        sscanf(p, "%f %f %f %f %f %f %f %f %n",
        &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
        &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
        &v[i].uv.x, &v[i].uv.y, &rd);
        v[i].col = 0xffffffff;
        p = (char*)((size_t)p + rd);
      }
#ifdef DEBUG
      kgmLog::log("\nEnd vertices");
#endif
    }
    else if(id == "Faces")
    {
      u32 count = 0;
      kgmMesh::Face_16*   f = 0;

      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      f = (kgmMesh::Face_16*)m->fAlloc(count);
      mnode->node(i)->data(data);

      char* p = data.data();

      for(int i = 0; i < count; i++)
      {
        u32 fi[3], rd;
        sscanf(p, "%i %i %i %n", &fi[0], &fi[1], &fi[2], &rd);
        f[i].a = fi[0];
        f[i].b = fi[1];
        f[i].c = fi[2];
        p = (char*)((size_t)p + rd);
      }
#ifdef DEBUG
      kgmLog::log("\nEnd faces");
#endif
    }
    else if(id == "Skin")
    {
      u32 count = 0;

      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);

      kgmMesh::Vertex_P_N_C_T* v = new kgmMesh::Vertex_P_N_C_T[ m->vcount() ];

      memcpy(v, m->vertices(), m->vcount() * m->vsize());

      kgmMesh::Vertex_P_N_C_T_BW_BI* s = (kgmMesh::Vertex_P_N_C_T_BW_BI*)m->vAlloc(count,kgmMesh::FVF_P_N_C_T_BW_BI);

      mnode->node(i)->data(data);

      char* p = data.data();

      for(int i = 0; i < count; i++)
      {
        u32  nw = 0, rd = 0;
        int   bi[4] = {0};
        float bw[4] = {0.0f};

        sscanf(p, "%i %i %i %i %f %f %f %f %n", //&nw,
        &bi[0], &bi[1], &bi[2], &bi[3],
        &bw[0], &bw[1], &bw[2], &bw[3], &rd);
        s[i].bw[0] = bw[0];
        s[i].bi[0] = bi[0];
        s[i].bw[1] = bw[1];
        s[i].bi[1] = bi[1];
        s[i].bw[2] = bw[2];
        s[i].bi[2] = bi[2];
        s[i].bw[3] = bw[3];
        s[i].bi[3] = bi[3];
        s[i].pos = v[i].pos;
        s[i].nor = v[i].nor;
        s[i].col = v[i].col;
        s[i].uv = vec2(v[i].uv.x, v[i].uv.y);
        p = (char*)((size_t)p + rd);
      }

      delete [] v;
#ifdef DEBUG
      kgmLog::log("\nEnd skin");
#endif
    }
  }


  for(int i = 0; i < materials.size(); i++)
    materials[i]->release();

  materials.clear();

  m->rebound();

  return m;
}

s32 kgmGameTools::genShapeCollision(kgmXml& x, kgmList<triangle3>& shape)
{
  kgmXml::Node* c_node = 0;

  if(!x.m_node || !(c_node = x.m_node->node("kgmCollision")))
    return 0;

  s32 p_count = 0;

  if(c_node->m_name == "kgmCollision")
  {
    kgmString val;

    c_node->attribute("polygons", val);

    for(int j = 0; j < c_node->nodes(); j++)
    {
      kgmXml::Node* node = c_node->node(j);

      if(node->m_name == "Polygon")
      {
        kgmString scount;
        u32       count;

        node->attribute("vertices", scount);
        count = kgmConvert::toInteger(scount);

        vec3*     pol = new vec3[count];
        vec3      v;
        int       n = 0;
        char*     pdata = node->m_data.data();

        for(int k = 0; k < count; k++)
        {
          sscanf(pdata, "%f %f %f%n", &v.x, &v.y, &v.z, &n);
          (pdata) += (u32)n;
          pol[k] = v;
        }

        for(int k = 2; k < count; k++)
        {
          shape.add(triangle3(pol[0], pol[k - 1], pol[k]));
          p_count++;
        }

        delete [] pol;
      }
    }
  }

  return p_count;
}

kgmShapeCollision* kgmGameTools::genShapeCollision(kgmXml& x)
{
  kgmXml::Node* c_node = 0;

  if(!x.m_node || !(c_node = x.m_node->node("kgmCollision")))
    return null;

  kgmShapeCollision* shape = null;

  if(c_node->m_name == "kgmCollision")
  {
    kgmString val;

    c_node->attribute("polygons", val);
    shape = new kgmShapeCollision();

    for(int j = 0; j < c_node->nodes(); j++)
    {
      kgmXml::Node* node = c_node->node(j);

      if(node->m_name == "Polygon")
      {
        kgmString scount;
        u32       count;

        node->attribute("vertices", scount);
        count = kgmConvert::toInteger(scount);

        vec3*     pol = new vec3[count];
        vec3      v;
        int       n = 0;
        char*     pdata = node->m_data.data();

        for(int k = 0; k < count; k++)
        {
          sscanf(pdata, "%f %f %f%n", &v.x, &v.y, &v.z, &n);
          (pdata) += (u32)n;
          pol[k] = v;
        }

        for(int k = 2; k < count; k++)
        {
          shape->triangles.add(triangle3(pol[0], pol[k - 1], pol[k]));
        }

        delete [] pol;
      }
    }
  }

  return shape;
}

kgmCollision::Shape* kgmGameTools::genShapeCollision(kgmMesh &m)
{
  kgmCollision::Shape* shape = null;

  if(m.faces() && m.fcount())
  {
    shape = new kgmCollision::Shape();

    for(int i = 0; i < m.fcount(); i++)
    {
      vec3 a, b, c;
      u32  ia, ib, ic;

      if(m.fff() == kgmMesh::FFF_16)
      {
        kgmMesh::Face_16 f;

        memcpy(&f, m.faces() + i * m.fsize(), m.fsize());

        ia = f.a;
        ib = f.b;
        ic = f.c;
      }
      else
      {
        kgmMesh::Face_32 f;

        memcpy(&f, m.faces() + i * m.fsize(), m.fsize());

        ia = f.a;
        ib = f.b;
        ic = f.c;
      }

      memcpy(&a, m.vertices() + ia * m.vsize(), m.vsize());
      memcpy(&b, m.vertices() + ib * m.vsize(), m.vsize());
      memcpy(&c, m.vertices() + ic * m.vsize(), m.vsize());

      shape->triangles.add(triangle3(a, b, c));
    }
  }
  else if(m.vertices() && m.vcount())
  {
    vec3 a, b, c;

    for(int i = 0; i < m.vcount(); i+=3)
    {
      memcpy(&a, m.vertices() + (i    ) * m.vsize(), m.vsize());
      memcpy(&b, m.vertices() + (i + 1) * m.vsize(), m.vsize());
      memcpy(&c, m.vertices() + (i + 2) * m.vsize(), m.vsize());

      shape->triangles.add(triangle3(a, b, c));
    }
  }

  return shape;
}

kgmGuiStyle* kgmGameTools::genGuiStyle(kgmIResources *rc, kgmString id)
{
#define KGM_STRHEX_TO_INT(s, i) if(s.length() > 0) sscanf(s, "%x", &i)

  kgmMemory<u8> m;

  if(rc->getFile((char*)id, m))
  {
    kgmXml xml(m);

    if(!xml.m_node)
      return null;

    kgmGuiStyle* gs = new kgmGuiStyle();

    for(int i = 0; i < xml.m_node->m_nodes.size(); i++)
    {
      kgmXml::Node* n = xml.m_node->m_nodes[i];
      kgmString     attr;

      if(n->m_name == "Font")
      {
        n->attribute("name", attr);
        gs->gui_font = rc->getFont(attr, 16, 16);
      }
      else if(n->m_name == "Gui")
      {
        n->attribute("bg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sgui.bg_color);
        n->attribute("fg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sgui.fg_color);
        n->attribute("ac_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sgui.ac_color);
        n->attribute("tx_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sgui.tx_color);
        n->attribute("image", attr);
        gs->sgui.image = rc->getTexture(attr);
      }
      else if(n->m_name == "GuiList")
      {
        n->attribute("bg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->slist.bg_color);
        n->attribute("fg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->slist.fg_color);
        n->attribute("ac_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->slist.ac_color);
        n->attribute("tx_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->slist.tx_color);
        n->attribute("image", attr);
        gs->slist.image = rc->getTexture(attr);
      }
      else if(n->m_name == "GuiButton")
      {
        n->attribute("bg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sbutton.bg_color);
        n->attribute("fg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sbutton.fg_color);
        n->attribute("ac_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sbutton.ac_color);
        n->attribute("tx_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sbutton.tx_color);
        n->attribute("image", attr);
        gs->sbutton.image = rc->getTexture(attr);
      }
      else if(n->m_name == "GuiMenu")
      {
        n->attribute("bg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->smenu.bg_color);
        n->attribute("fg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->smenu.fg_color);
        n->attribute("ac_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->smenu.ac_color);
        n->attribute("tx_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->smenu.tx_color);
        n->attribute("image", attr);
        gs->smenu.image = rc->getTexture(attr);
      }
      else if(n->m_name == "GuiText")
      {
        n->attribute("bg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->stext.bg_color);
        n->attribute("fg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->stext.fg_color);
        n->attribute("ac_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->stext.ac_color);
        n->attribute("tx_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->stext.tx_color);
        n->attribute("image", attr);
        gs->stext.image = rc->getTexture(attr);
      }
      else if(n->m_name == "GuiScroll")
      {
        n->attribute("bg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sscroll.bg_color);
        n->attribute("fg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sscroll.fg_color);
        n->attribute("ac_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sscroll.ac_color);
        n->attribute("tx_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->sscroll.tx_color);
        n->attribute("image", attr);
        gs->sscroll.image = rc->getTexture(attr);
      }
      else if(n->m_name == "GuiLabel")
      {
        n->attribute("bg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->slabel.bg_color);
        n->attribute("fg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->slabel.fg_color);
        n->attribute("ac_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->slabel.ac_color);
        n->attribute("tx_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->slabel.tx_color);
        n->attribute("image", attr);
        gs->slabel.image = rc->getTexture(attr);
      }
      else if(n->m_name == "GuiCheck")
      {
        n->attribute("bg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->scheck.bg_color);
        n->attribute("fg_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->scheck.fg_color);
        n->attribute("ac_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->scheck.ac_color);
        n->attribute("tx_color", attr);
        KGM_STRHEX_TO_INT(attr, gs->scheck.tx_color);
        n->attribute("bg_check", attr);
        KGM_STRHEX_TO_INT(attr, gs->scheck.bg_check);
        n->attribute("fg_check", attr);
        KGM_STRHEX_TO_INT(attr, gs->scheck.fg_check);
        n->attribute("image", attr);
        gs->scheck.image = rc->getTexture(attr);
      }
    }

    return gs;
  }

  return null;

#undef KGM_STRHEX_TO_INT
}

//SOUNDS
kgmSound* kgmGameTools::genSound(kgmIAudio* au, kgmMemory<u8>& m){
  kgmWave wav;

  if(!au || !wav.from_mem((char*)m.data(), m.length()))
    return 0;

  kgmIAudio::FMT fmt;

  switch(wav.fmt_format.nChannels){
  case 1:
    if(wav.fmt_format.wBitsPerSample == 8){
      fmt = kgmIAudio::FMT_MONO8;
    }

    if(wav.fmt_format.wBitsPerSample == 16){
      fmt = kgmIAudio::FMT_MONO16;
    }
    break;
  case 2:
    if(wav.fmt_format.wBitsPerSample == 8){
      fmt = kgmIAudio::FMT_STEREO8;
    }

    if(wav.fmt_format.wBitsPerSample == 16){
      fmt = kgmIAudio::FMT_STEREO16;
    }
    break;
  default:
    return 0;
  }

  kgmIAudio::Sound s = au->create(fmt,
  wav.fmt_format.nSamplesPerSec,
  wav.data_size,
  wav.data);

  if(!s)
    return 0;

  kgmSound* snd = new kgmSound(s);

  return snd;
}

kgmParticles* kgmGameTools::genParticles(kgmXml &x)
{
  return null;
}

kgmMesh* kgmGameTools::parseMesh(kgmXml::Node& node)
{
  kgmMesh* m = 0;

  kgmXml::Node* mnode = 0;

  if(node.m_name != "kgmMesh")
    return null;

  m = new kgmMesh();
  mnode->attribute("id", m->m_id);

  for(int i = 0; i < mnode->nodes(); i++){
    kgmString id, val, data;

    if(mnode->node(i))
      mnode->node(i)->id(id);
    else
      break;

    if(id == "Vertices"){
      u32 count = 0;
      kgmMesh::Vertex_P_N_C_T*  v = 0;

      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      v = (kgmMesh::Vertex_P_N_C_T*)m->vAlloc(count, kgmMesh::FVF_P_N_C_T);
      mnode->node(i)->data(data);
      char* p = data.data();

      for(int i = 0; i < count; i++)
      {
        u32 rd;
        sscanf(p, "%f %f %f %f %f %f %f %f %n",
        &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
        &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
        &v[i].uv.x, &v[i].uv.y, &rd);
        v[i].col = 0xffffffff;
        p = (char*)((size_t)p + rd);
      }
    }
    else if(id == "Faces")
    {
      u32 count = 0;
      kgmMesh::Face_16*   f = 0;
      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      f = (kgmMesh::Face_16*)m->fAlloc(count);
      mnode->node(i)->data(data);
      char* p = data.data();

      for(int i = 0; i < count; i++)
      {
        u32 fi[3], rd;
        sscanf(p, "%i %i %i %n", &fi[0], &fi[1], &fi[2], &rd);
        f[i].a = fi[0];
        f[i].b = fi[1];
        f[i].c = fi[2];
        p = (char*)((size_t)p + rd);
      }
    }
    else if(id == "Skin")
    {
      u32 count = 0;
      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      kgmMesh::Vertex_P_N_C_T* v = new kgmMesh::Vertex_P_N_C_T[ m->vcount() ];
      memcpy(v, m->vertices(), m->vcount() * m->vsize());
      kgmMesh::Vertex_P_N_C_T_BW_BI* s = (kgmMesh::Vertex_P_N_C_T_BW_BI*)m->vAlloc(count,kgmMesh::FVF_P_N_C_T_BW_BI);
      mnode->node(i)->data(data);

      char* p = data.data();

      for(int i = 0; i < count; i++)
      {
        u32  nw = 0, rd = 0;
        int   bi[4] = {0};
        float bw[4] = {0.0f};
        sscanf(p, "%i %i %i %i %f %f %f %f %n",
        &bi[0], &bi[1], &bi[2], &bi[3],
        &bw[0], &bw[1], &bw[2], &bw[3], &rd);
        s[i].bw[0] = bw[0];
        s[i].bi[0] = bi[0];
        s[i].bw[1] = bw[1];
        s[i].bi[1] = bi[1];
        s[i].bw[2] = bw[2];
        s[i].bi[2] = bi[2];
        s[i].bw[3] = bw[3];
        s[i].bi[3] = bi[3];
        s[i].pos = v[i].pos;
        s[i].nor = v[i].nor;
        s[i].col = v[i].col;
        s[i].uv = vec2(v[i].uv.x, v[i].uv.y);
        p = (char*)((size_t)p + rd);
      }

      delete [] v;
    }
  }

  return m;
}

kgmLight* kgmGameTools::parseLight(kgmXml::Node& node)
{

}

kgmMaterial* kgmGameTools::parseMaterial(kgmXml::Node& node)
{

}

kgmSkeleton* kgmGameTools::parseSkeleton(kgmXml::Node& node)
{

}

bool kgmGameTools::initActor(kgmIGame* game, kgmActor *actor, kgmString id)
{
  kgmMemory<u8> mem;

  game->getResources()->getFile(id, mem);

  kgmXml xml(mem);

  mem.clear();

  return kgmGameTools::initActor(game, actor, xml);
}

bool kgmGameTools::initActor(kgmIGame* game, kgmActor *actor, kgmXml &xml)
{
  if(!game || !actor || !xml.m_node)
    return false;

  kgmXml::Node* a_node = null;

  for(int i = 0; i < xml.m_node->nodes(); i++)
  {
    kgmString id;
    xml.m_node->node(i)->id(id);

    if(id == "kgmActor")
    {
      a_node = xml.m_node->node(i);

      break;
    }
  }

  if(!a_node)
    return false;

  for(int i = 0; i < a_node->nodes(); ++i){
    kgmString id, val;

    if(a_node->node(i))
      a_node->node(i)->id(id);
    else
      break;

    if(id == "Mass")
    {
      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f", &actor->getBody()->m_mass);
    }
    else if(id == "Bound")
    {
      float a[3];
      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f%f%f", &a[0], &a[1], &a[2]);
      actor->getBody()->m_bound.min = vec3(-0.5 * a[0], -0.5 * a[1], 0.0);
      actor->getBody()->m_bound.max = vec3( 0.5 * a[0],  0.5 * a[1], a[2]);
    }
    else if(id == "Collision")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "convex")
      {
        kgmMemory<u8> mem;
        kgmString     dfile;

        a_node->node(i)->attribute("data", dfile);

        if(game->getResources()->getFile(dfile, mem))
        {
          kgmXml xml(mem);

          if(xml.m_node)
          {
            for(int i = 0; i < xml.m_node->nodes(); i++)
            {
              kgmXml::Node* node = xml.m_node->node(i);

              if(node->m_name == "kgmCollision")
              {
                node->attribute("polygons", val);

                for(int j = 0; j < xml.m_node->node(i)->nodes(); j++)
                {
                  kgmXml::Node* node = xml.m_node->node(i)->node(j);

                  if(node->m_name == "Polygon")
                  {
                    kgmString scount;
                    u32       count;

                    node->attribute("vertices", scount);
                    count = kgmConvert::toInteger(scount);

                    vec3*     pol = new vec3[count];
                    vec3      v;
                    int       n = 0;
                    char*     pdata = node->m_data.data();

                    for(int k = 0; k < count; k++)
                    {
                      sscanf(pdata, "%f %f %f%n", &v.x, &v.y, &v.z, &n);
                      (pdata) += (u32)n;
                      pol[k] = v;
                    }

                    for(int k = 2; k < count; k++)
                    {
                      vec3 v[3] = {pol[0], pol[k - 1], pol[k]};

                      actor->getBody()->addShapeSide(v);
                    }

                    delete [] pol;
                  }
                }
              }
            }
          }
        }

        if(actor->getBody()->m_convex.size() > 0)
          actor->getBody()->m_shape == kgmBody::ShapePolyhedron;
      }
    }
    else if(id == "Gravity")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "true")
        actor->getBody()->m_gravity = true;
      else
        actor->getBody()->m_gravity = false;
    }
    else if(id == "Dummies")
    {
      kgmMemory<u8> mem;
      a_node->node(i)->attribute("value", val);

      if((val.length() > 0) && game->getResources()->getFile(val, mem))
      {
        kgmXml xml(mem);

        if(xml.m_node)
        {
          for(int i = 0; i < xml.m_node->nodes(); i++)
          {
            kgmXml::Node* node = xml.m_node->node(i);

            if(!node)
              break;

            if(node->m_name == "kgmDummy")
            {
              kgmDummy* dummy = new kgmDummy();

              actor->add(dummy);
              dummy->release();
              node->attribute("name", dummy->m_id);

              for(int j = 0; j < node->nodes(); j++)
              {
                kgmXml::Node* jnode = node->node(j);

                if(!jnode)
                  break;

                if(jnode->m_name == "Position")
                {
                  kgmString spos;
                  vec3      v;

                  jnode->attribute("value", spos);
                  sscanf(spos, "%f %f %f", &v.x, &v.y, &v.z);
                  dummy->setShift(v);
                }
                else if(jnode->m_name == "Rotation")
                {
                  kgmString srot;
                  vec3      r;

                  jnode->attribute("value", srot);
                  sscanf(srot, "%f %f %f", &r.x, &r.y, &r.z);
                  dummy->setOrient(r);
                }
              }
            }
          }

          xml.close();
        }

        mem.clear();
      }
    }
    else if(id == "Visual")
    {
      kgmMesh*      msh = 0;
      kgmMaterial*  mtl = 0;
      kgmSkeleton*  skl = 0;
      kgmAnimation* anm = 0;

      for(int j = 0; j < a_node->node(i)->nodes(); j++)
      {
        if(a_node->node(i)->node(j))
          a_node->node(i)->node(j)->id(id);
        else
          break;

        if(id == "Material"){
          a_node->node(i)->node(j)->attribute("value", val);
          mtl = game->getResources()->getMaterial(val);
        }else if(id == "Mesh"){
          a_node->node(i)->node(j)->attribute("value", val);
          msh = game->getResources()->getMesh(val);
        }else if(id == "Animation"){
          a_node->node(i)->node(j)->attribute("value", val);
          anm = game->getResources()->getAnimation(val);
        }else if(id == "Skeleton"){
          a_node->node(i)->node(j)->attribute("value", val);
          skl = game->getResources()->getSkeleton(val);
        }else if(id == "Dummy"){
          a_node->node(i)->node(j)->attribute("value", val);
        }
      }

      if(msh)
      {
        actor->getVisual()->set(msh);
        actor->getVisual()->set(mtl);
        actor->getVisual()->setAnimation(anm);
        actor->getVisual()->setSkeleton(skl);

        msh->release();

        if(mtl) mtl->release();
        if(skl) skl->release();
        if(anm) anm->release();

        msh = null;
        mtl = null;
        skl = null;
        anm = null;
      }
    }
    else if(id == "Input")
    {
      u32 btn = 0, btn1 = 0, btn2 = 0, stat = 0;
      kgmString state;

      a_node->node(i)->attribute("button",  val);  sscanf(val, "%i", &btn);
      val = "";
      a_node->node(i)->attribute("button1", val);  sscanf(val, "%i", &btn1);
      val = "";
      a_node->node(i)->attribute("button2", val);  sscanf(val, "%i", &btn2);
      val = "";
      a_node->node(i)->attribute("status",  val);  sscanf(val, "%i", &stat);
      a_node->node(i)->attribute("state",   state);

      actor->add(btn, stat, state, btn1, btn2);
    }
    else if(id == "InputActive")
    {
      u32 btn = 0, btn1 = 0, btn2 = 0, stat = 1;
      kgmString state;

      a_node->node(i)->attribute("button",  val);  sscanf(val, "%i", &btn);
      val = "";
      a_node->node(i)->attribute("button1", val);  sscanf(val, "%i", &btn1);
      val = "";
      a_node->node(i)->attribute("button2", val);  sscanf(val, "%i", &btn2);
      a_node->node(i)->attribute("state",   state);

      actor->add(btn, stat, state, btn1, btn2, true);
    }
    else if(id == "State")
    {
      kgmString s;
      kgmActor::State* state = new kgmActor::State();

      state->sound = null;
      state->animation = null;

      a_node->node(i)->attribute("id", state->id);
      a_node->node(i)->attribute("type", state->type);
      a_node->node(i)->attribute("switch", state->switchto);

      a_node->node(i)->attribute("time", s);
      if(s.length() > 0) sscanf(s, "%i", &state->timeout);

      a_node->node(i)->attribute("priority", s);
      if(s.length() > 0) sscanf(s, "%i", &state->priopity);

      for(int j = 0; j < a_node->node(i)->nodes(); j++)
      {
        if(a_node->node(i)->node(j))
          a_node->node(i)->node(j)->id(id);
        else
          break;

        if(id == "Sound")
        {
          a_node->node(i)->node(j)->attribute("value", s);
          state->sound = game->getResources()->getSound(s);
        }
        else if(id == "Animation")
        {
          a_node->node(i)->node(j)->attribute("value", s);
          state->animation = game->getResources()->getAnimation(s);
          a_node->node(i)->node(j)->attribute("start", s);
          if(s.length() > 0) sscanf(s, "%i", &state->fstart);
          a_node->node(i)->node(j)->attribute("end", s);
          if(s.length() > 0) sscanf(s, "%i", &state->fend);
        }
        else if(id == "Option")
        {
          kgmString k, v;
          a_node->node(i)->node(j)->attribute("key", k);
          a_node->node(i)->node(j)->attribute("value", v);
        }
      }
      actor->m_states.add(state);
    }
    else
    {
      kgmString val;

      if(a_node->node(i)->attribute("value", val))
      {
        actor->m_options.add(id, val);
      }
    }
  }

  return true;
}
