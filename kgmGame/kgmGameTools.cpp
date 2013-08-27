#include "kgmGameTools.h"
#include "kgmIGame.h"

#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmMemory.h"
#include "../kgmBase/kgmIGraphics.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmMath/kgmRect2d.h"
#include "../kgmMedia/kgmSound.h"
#include "../kgmGraphics/kgmPicture.h"

static char str_buf[1024];

kgmGameTools::kgmGameTools(){
}

kgmGameTools::~kgmGameTools(){
}

//*************** DRAWING ***************
void kgmGameTools::gcDrawRect(kgmIGraphics* gc, int x, int y, int w, int h, u32 col, void* tex){
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

void kgmGameTools::gcDrawText(kgmIGraphics* gc, kgmFont* font, int fw, int fh, int x, int y, int w, int h, u32 col, kgmString& text){
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

kgmPicture* kgmGameTools::genPicture(kgmMemory<char>& m)
{
  u32 id_tga = 0x00020000;
  if(m.empty())
    return 0;
  if(!memcmp("BM", m.data(), 2))
    return genPictureFromBmp(m);
  if(!memcmp(&id_tga, m.data(), 4))
    return genPictureFromTga(m);
  return 0;
}

kgmPicture* kgmGameTools::genPictureFromBmp(kgmMemory<char>& m)
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

  if(b_btcnt == 8){
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

kgmPicture* kgmGameTools::genPictureFromTga(kgmMemory<char>& m)
{
  if(m.empty())
    return 0;

  uchar   idl, cmp, dt, btcnt, dsc;
  uchar   clmap[5];
  u16  xorgn, yorgn, w, h;
  char*   pm = (char*)m.data();

  int   width = 0;
  int   height = 0;
  int   bpp = 0;
  int   frames = 0;
  void* pdata = 0;

  memcpy(&idl, pm, 1);      pm += 1;
  memcpy(&cmp, pm, 1);      pm += 1;
  memcpy(&dt, pm, 1);       pm += 1;
  memcpy(&clmap, pm, 5);    pm += 5;
  memcpy(&xorgn, pm, 2);    pm += 2;
  memcpy(&yorgn, pm, 2);    pm += 2;
  memcpy(&w, pm, 2);        pm += 2;
  memcpy(&h, pm, 2);        pm += 2;
  memcpy(&btcnt, pm, 1);    pm += 1;
  memcpy(&dsc, pm, 1);      pm += 1;

  if(dt != 2)
    return 0;

  if((btcnt != 24))
    if((btcnt != 32))
      return 0;

  u32 r_size = w * h * (btcnt/8);
  pm += idl;
  pdata = malloc(sizeof(char) * r_size);
  memcpy(pdata, pm, r_size);
  u32 k = (u32)((u32*)pdata)[0];
  width = w;
  height = h;
  bpp = btcnt;

  for(int i = 0; i < (w * h); i++)
  {
    char* pt = (char*)(((char*)pdata) + i * btcnt / 8);
    char  t  = pt[0];
    pt[0] = pt[2];
    pt[2] = t;
  }
  
  return new kgmPicture(width, height, bpp, frames, pdata);
}

kgmTexture* kgmGameTools::genTexture(kgmIGraphics* gc, kgmMemory<char>& m)
{
  kgmPicture* pic = genPicture(m);
  if(!pic)
    return 0;
  u32 fmt = gctex_fmt32;
  switch(pic->bpp){
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
  kgmTexture* tex = new kgmTexture;
  tex->m_texture = gc->gcGenTexture(pic->pdata, pic->width, pic->height, fmt, gctype_tex2d);
  pic->release();

  if(!tex->m_texture){
    tex->release();
    return 0;
  }
  return tex;
}

kgmFont* kgmGameTools::genFont(kgmIGraphics* gc, u32 w, u32 h, u32 r, u32 c, kgmMemory<char>& m)
{
  kgmPicture* pic = genPicture(m);

  if(!pic)
    return 0;

  u32 fmt = gctex_fmt32;

  switch(pic->bpp){
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
  font->m_texture  = gc->gcGenTexture(pic->pdata, pic->width, pic->height, fmt, gctype_tex2d);
  pic->release();

  if(!font->m_texture){
    font->release();

    return 0;
  }

  font->m_rows = r;
  font->m_cols = c;
  return font;
}

// SHADER
kgmShader* kgmGameTools::genShader(kgmIGraphics* gc, kgmString& s){
  char* mem_vsh = 0;
  char* mem_fsh = 0;

  if(!gc || s.empty())
    return 0;

  mem_fsh = (char*)strstr((char*)s, "//Fragment Shader");
  if(mem_fsh){
    u32 sh_s = (u32)mem_fsh - (u32)s.data();
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

kgmShader* kgmGameTools::genShader(kgmIGraphics* gc, kgmXml& xml){
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

kgmMaterial* kgmGameTools::genMaterial(kgmMemory<char>& m){
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

  while(!m.eof()){
    memset(str, 0, 1024);
    memset(key, 0, 128);
    memset(val, 0, 128);

    m.reads(str, 1024, "\n", 1);
    sscanf(str, "%s %s", key, val);

    if(!strcmp(key, "color")){
      sscanf(str, "%s %f %f %f %f", key, &farr[0], &farr[1], &farr[2], &farr[3]);
      mtl->m_color = kgmMaterial::Color(farr[0], farr[1], farr[2], farr[3]);
    }
    if(!strcmp(key, "map_texture")){
      mtl->m_tex_color = kgmIGame::getGame()->getResources()->getTexture(val);
    }
    if(!strcmp(key, "shader")){
      if(val == "SKIN")
        mtl->m_shader = kgmMaterial::ShaderSkin;
      else if(val == "WATER")
        mtl->m_shader = kgmMaterial::ShaderWater;
      else if(val == "MIRROR")
        mtl->m_shader = kgmMaterial::ShaderMirror;
      else if(val == "ICE")
        mtl->m_shader = kgmMaterial::ShaderIce;
      //mtl->m_shader = kgmIGame::getGame()->getResources()->getShader(val);
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
      mtl->m_tex_color = kgmIGame::getGame()->getResources()->getTexture(val);
      kgm_log() << "genMaterial: map color " << (s32)mtl->m_tex_color;
    }
    else if(id == "map_normal")
    {
      mnode->node(i)->attribute("value", val);
      mtl->m_tex_normal = kgmIGame::getGame()->getResources()->getTexture(val);
    }
    else if(id == "map_specular")
    {
      mnode->node(i)->attribute("value", val);
      mtl->m_tex_specular = kgmIGame::getGame()->getResources()->getTexture(val);
    }
    else if(id == "Shader")
    {
      mnode->node(i)->attribute("value", val);
      if(val == "SKIN")
        mtl->m_shader = kgmMaterial::ShaderSkin;
      else if(val == "WATER")
        mtl->m_shader = kgmMaterial::ShaderWater;
      else if(val == "MIRROR")
        mtl->m_shader = kgmMaterial::ShaderMirror;
      else if(val == "ICE")
        mtl->m_shader = kgmMaterial::ShaderIce;
      else
        mtl->m_shader = kgmMaterial::ShaderBase;
    }
  }

  return mtl;
}


//SKELETON
kgmSkeleton* kgmGameTools::genSkeleton(kgmMemory<char>& m){
  kgmSkeleton* skel;
  char* str = 0;
  char* key = 0;
  char* val = 0;
  char* p   = (char*)m;
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

    if(0 == m.reads(str, 1024, "\n", 1))
      break;

    sscanf(str, "%s %s", key, val);

    if(!strcmp(key, "Bone")){
      kgmSkeleton::Joint* joint = new kgmSkeleton::Joint;
      char* c = strstr(str, "Bone");
      if(c){
        c += 5;
        strncpy(joint->n, c, sizeof(joint->n));
      }
      memset(str, 0, 1024);
      if(0 == m.reads(str, 1024, "\n", 1))
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
kgmAnimation* kgmGameTools::genAnimation(kgmMemory<char>& m){
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

    if(0 == m.reads(str, 512, "\n", 1))
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
kgmMesh* kgmGameTools::genMesh(kgmMemory<char>& mm){
  kgmList<kgmMaterial*> materials;
  kgmMaterial* mtl = 0;

  kgmMesh* m = 0;
  char* key = 0;
  char* val = 0;
  char* str = 0;
  int   col[4] = {0};

  if(mm.empty())
    return 0;
  key = new char[128];
  val = new char[128];
  str = new char[512];

  while(!mm.eof()){
    memset(key, 0, 128);
    memset(val, 0, 128);
    memset(str, 0, 512);
    mm.reads(str, 512, "\n", 1);
    sscanf(str, "%s", key);
    if(!strcmp(key, "Mesh")){
      sscanf(str, "%s %s", key, val);
      if(m)
        break;
      m = new kgmMesh();
      strcpy(m->m_id, val);
    }
    if(!strcmp(key, "Vertices")){
      u32 count = 0;
      u32 maps  = 0;
      kgmMesh::Vertex_P_N_C_T2*  v = 0;
      sscanf(str, "%s %i %i", key, &count, &maps);
      v = (kgmMesh::Vertex_P_N_C_T2*)m->vAlloc(count, kgmMesh::FVF_P_N_C_T2);
      for(int i = 0; i < count; i++){
        mm.reads(str, 512, "\n", 1);
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
      for(int i = 0; i < count; i++){
        u32 fi[3];
        mm.reads(str, 512, "\n", 1);
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
      for(int i = 0; i < count; i++){
        u32  nw = 0;
        u32  bi[4] = {0};
        float bw[4] = {0.0f};
        mm.reads(str, 512, "\n", 1);
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
    return 0;

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
      for(int i = 0; i < count; i++){
        u32 rd;
        sscanf(p, "%f %f %f %f %f %f %f %f %n",
        &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
        &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
        &v[i].uv.x, &v[i].uv.y, &rd);
        v[i].col = 0xffffffff;
        p = (char*)((u32)p + rd);
      }
      kgmLog::log("\nEnd vertices");
    }else if(id == "Faces"){
      u32 count = 0;
      kgmMesh::Face_16*   f = 0;
      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      f = (kgmMesh::Face_16*)m->fAlloc(count);
      mnode->node(i)->data(data);
      char* p = data.data();
      for(int i = 0; i < count; i++){
        u32 fi[3], rd;
        sscanf(p, "%i %i %i %n", &fi[0], &fi[1], &fi[2], &rd);
        f[i].a = fi[0];
        f[i].b = fi[1];
        f[i].c = fi[2];
        p = (char*)((u32)p + rd);
      }
      kgmLog::log("\nEnd faces");
    }else if(id == "Skin"){
      u32 count = 0;
      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      kgmMesh::Vertex_P_N_C_T* v = new kgmMesh::Vertex_P_N_C_T[ m->vcount() ];
      memcpy(v, m->vertices(), m->vcount() * m->vsize());
      kgmMesh::Vertex_P_N_C_T_BW_BI* s = (kgmMesh::Vertex_P_N_C_T_BW_BI*)m->vAlloc(count,kgmMesh::FVF_P_N_C_T_BW_BI);
      mnode->node(i)->data(data);
      char* p = data.data();
      for(int i = 0; i < count; i++){
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
        p = (char*)((u32)p + rd);
      }
      delete [] v;
      kgmLog::log("\nEnd skin");
    }
  }


  for(int i = 0; i < materials.size(); i++)
    materials[i]->release();
  materials.clear();
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

kgmGuiStyle* kgmGameTools::genGuiStyle(kgmIResources *rc, kgmString id)
{
#define KGM_STRHEX_TO_INT(s, i) if(s.length() > 0) sscanf(s, "%x", &i)

  kgmMemory<char> m;

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
    }

    return gs;
  }

  return null;

#undef KGM_STRHEX_TO_INT
}

//SOUNDS
kgmSound* kgmGameTools::genSound(kgmIAudio* au, kgmMemory<char>& m){
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

  kgmIAudio::Sound* s = au->create(fmt,
  wav.fmt_format.nSamplesPerSec,
  wav.data_size,
  wav.data);

  if(!s)
    return 0;

  kgmSound* snd = new kgmSound();
  snd->m_sound = s;

  return snd;
}
