#include "kgmGameTools.h"
#include "kgmIGame.h"

#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmArray.h"
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmArrayStream.h"
#include "../kgmBase/kgmVariable.h"
#include "../kgmMath/kgmRect2d.h"
#include "../kgmMedia/kgmSound.h"
#include "../kgmGraphics/kgmDummy.h"
#include "../kgmGraphics/kgmPicture.h"
#include "../kgmGraphics/kgmGraphics.h"
#include "../kgmGraphics/kgmParticles.h"

#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiFrame.h"
#include "../kgmGraphics/kgmGuiLayout.h"

#include "../kgmUtils/kgmTga.h"

#include "../kgmAI/kgmIAI.h"

#include "kgmGameApp.h"
#include "kgmGameScript.h"

#include "kgmUnit.h"

kgmGameTools::kgmGameTools()
{
}

kgmGameTools::~kgmGameTools()
{
}

kgmPicture* kgmGameTools::genPicture(kgmArray<u8>& m)
{
  u32 id_tga = 0x00020000;

  u32 id_tga_comp = 0x000a0000;

  if(m.empty() || !m.data())
    return 0;

  if(!memcmp("BM", m.data(), 2))
    return genPictureFromBmp(m);

  if(!memcmp(&id_tga, m.data(), 4) || !memcmp(&id_tga_comp, m.data(), 4))
    return genPictureFromTga(m);

  return null;
}

kgmPicture* kgmGameTools::genPictureFromBmp(kgmArray<u8>& m)
{
  if(m.empty())
    return 0;

  u16 f_type = 0;
  u32 f_size = 0;
  u16 f_res0 = 0, f_res1 = 0;
  u32 f_boffs = 0;
  u32 b_size = 0;
  long b_width = 0, b_height = 0;
  u16  b_planes = 0, b_btcnt = 0;
  u32  b_compr = 0, b_sizeimg = 0;
  long b_xppm = 0, b_yppm = 0;
  u32  b_clus = 0, b_climp = 0;
  char* pm = (char*) m.data();

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
    pdata = (u32*)kgm_alloc(sizeof(u32) * r_size);
    u32 *pal = (u32*)kgm_alloc(sizeof(u32) * 256);
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
    kgm_free(pal);

    return new kgmPicture(width, height, bpp, frames, pdata);
  }

  u32 r_size = b_width * b_height * (b_btcnt / 8);
  pdata = kgm_alloc(sizeof(char) * r_size);
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

kgmPicture* kgmGameTools::genPictureFromTga(kgmArray<u8>& m)
{
  if(m.empty())
    return 0;

  {
    kgmTga* tga = new kgmTga();

    if (!tga->create(m))
      return null;

    kgmPicture* pic = tga->toPicture();

    delete tga;

    return pic;
  }

  uchar idl, cmp, dt, btcnt, dsc;
  uchar clmap[5];
  u16   xorgn, yorgn, w, h;
  u8*   pm = (u8*)m.data();

  int   width = 0;
  int   height = 0;
  int   bpp = 0;
  int   frames = 0;
  u8*   pdata = 0;

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

  bool compr;

  if(dt == 2)
    compr = false;
  else if (dt == 10)
    compr = true;
  else
    return 0;

  if((btcnt != 16) && (btcnt != 24) && (btcnt != 32))
    return 0;

  u32 bt_pp = btcnt / 8;

  if (bt_pp < 3)
    bt_pp = 3;

  u32 r_size = w * h * bt_pp;
  pm += idl;
  pdata = (u8*) kgm_alloc(sizeof(char) * r_size);
  //memcpy(pdata, pm, r_size);

  u32 pixels = w * h;

  if (compr)
  {
    u8* ptm = pm;

    typedef u8 pixel_t[4];

    u32 i = 0;

    while(i < pixels)
    {
      u8 rle = ptm[0]; ptm += 1;
      u8 cnt = rle & 0x7f;

      pixel_t t_px;

      memcpy(t_px, ptm, bt_pp);

      switch(bt_pp)
      {
      case 2:
        pdata[3 * i + 0] = ( ptm[0] & 0x1f) << 3;
        pdata[3 * i + 1] = ((ptm[0] & 0xe0) >> 2) | ((ptm[1] & 0x03) << 6);
        pdata[3 * i + 2] = ( ptm[1] & 0x7c) << 1;
        break;
      case 3:
        pdata[3 * i + 0] = ptm[0];
        pdata[3 * i + 1] = ptm[1];
        pdata[3 * i + 2] = ptm[2];
        break;
      case 4:
        pdata[4 * i + 0] = ptm[0];
        pdata[4 * i + 1] = ptm[1];
        pdata[4 * i + 2] = ptm[2];
        pdata[4 * i + 3] = ptm[3];
        break;
      }

      i++;

      if (cnt == 0)
      {
        ptm += bt_pp;
        continue;
      }

      if (rle & 0x80)
      {
        for (u32 k = 0; k < cnt; k++)
        {
          switch(bt_pp)
          {
          case 2:
            pdata[3 * i + 0] = ( ptm[0] & 0x1f) << 3;
            pdata[3 * i + 1] = ((ptm[0] & 0xe0) >> 2) | ((ptm[1] & 0x03) << 6);
            pdata[3 * i + 2] = ( ptm[1] & 0x7c) << 1;
            break;
          case 3:
            pdata[3 * i + 0] = ptm[0];
            pdata[3 * i + 1] = ptm[1];
            pdata[3 * i + 2] = ptm[2];
            break;
          case 4:
            pdata[4 * i + 0] = ptm[0];
            pdata[4 * i + 1] = ptm[1];
            pdata[4 * i + 2] = ptm[2];
            pdata[4 * i + 3] = ptm[3];
            break;
          }

          i++;
        }

        ptm += bt_pp;
      }
      else
      {
        for (u32 k = 0; k < cnt; k++)
        {
          memcpy(t_px, ptm, bt_pp);

          switch(bt_pp)
          {
          case 2:
            pdata[3 * i + 0] =  (ptm[0] & 0x1f) << 3;
            pdata[3 * i + 1] = ((ptm[0] & 0xe0) >> 2) | ((ptm[1] & 0x03) << 6);
            pdata[3 * i + 2] =  (ptm[1] & 0x7c) << 1;
            break;
          case 3:
            pdata[3 * i + 0] = ptm[0];
            pdata[3 * i + 1] = ptm[1];
            pdata[3 * i + 2] = ptm[2];
            break;
          case 4:
            pdata[3 * i + 0] = ptm[0];
            pdata[3 * i + 1] = ptm[1];
            pdata[3 * i + 2] = ptm[2];
            pdata[3 * i + 3] = ptm[3];
            break;
          }

          i++;
          ptm += bt_pp;
        }
      }
    }
  }
  else
  {
    if (btcnt == 16)
    {
      for(u32 i = 0; i < pixels; i++)
      {
        pdata[3 * i + 0] = ( pm[2 * i + 0] & 0x1f) << 3;
        pdata[3 * i + 1] = ((pm[2 * i + 0] & 0xe0) >> 2) | ((pm[2 * i + 1] & 0x03) << 6);
        pdata[3 * i + 2] = ( pm[2 * i + 1] & 0x7c) << 1;
      }
    }
    else
    {
      memcpy(pdata, pm, r_size);
    }
  }

  width  = w;
  height = h;
  bpp    = btcnt;

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

kgmTexture* kgmGameTools::genTexture(kgmIGC* gc, kgmArray<u8>& m)
{
  kgmPicture* pic = genPicture(m);

  if(!pic)
  {
    return null;
  }
  else
  {
    int nwidth, nheight;

    switch((u32) kgmGraphics::textureQuality)
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

  void* t = gc->gcGenTexture(pic->pdata, pic->width, pic->height, fmt, gctype_tex2d);

  kgmTexture* tex = new kgmTexture(t);

  delete pic;

  if(!tex->texture())
  {
    delete tex;

    return null;
  }

  return tex;
}

kgmFont* kgmGameTools::genFont(kgmIGC* gc, u32 w, u32 h, u32 r, u32 c, kgmArray<u8>& m)
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


  gc->gcSet(gcpar_texfltmag, (void*)gcflt_near);
  gc->gcSet(gcpar_texfltmin, (void*)gcflt_near);

  void* t  = gc->gcGenTexture(pic->pdata, pic->width, pic->height, fmt, gctype_tex2d);

  gc->gcSet(gcpar_texfltmag, (void*)gcflt_linear);
  gc->gcSet(gcpar_texfltmin, (void*)gcflt_linear);

  delete pic;

  kgmFont* font = new kgmFont(t, t, t);

  if(!font->texture())
  {
    delete font;

    return 0;
  }

  font->m_rows = r;
  font->m_cols = c;

  return font;
}

// SHADER
kgmShader* kgmGameTools::genShader(kgmIGC* gc, kgmString& s)
{
  char* mem_vsh = 0;
  char* mem_fsh = 0;

  char* separator = (char*) "//Fragment Shader";

  if(!gc || s.empty())
    return null;

  mem_fsh = (char*)strstr((char*)s, separator);

  if(mem_fsh)
  {
    size_t sh_s = (size_t)mem_fsh - (size_t)s.data();
    mem_vsh = (char*)kgm_alloc(sh_s + 1);
    memcpy(mem_vsh, s.data(), sh_s);
    mem_vsh[sh_s] = '\0';
    sh_s = s.length() - sh_s;
    char* fsh = mem_fsh;
    mem_fsh = (char*)kgm_alloc(sh_s + 1);
    memcpy(mem_fsh, fsh, sh_s);
    mem_fsh[sh_s] = '\0';
  }
  else
  {
    mem_vsh = (char*)kgm_alloc(s.length() + 1);
    memcpy(mem_vsh, s.data(), s.length());
    mem_vsh[s.length()] = '\0';
  }

  kgmArray<u8> vsource, fsource;

  kgmString vstr, fstr;

  vstr = kgmString(mem_vsh);
  fstr = kgmString(mem_fsh);

  if(mem_vsh)
    kgm_free(mem_vsh);

  if(mem_fsh)
    kgm_free(mem_fsh);

  vsource.alloc((u8*) vstr.data(), vstr.length() + 1);
  fsource.alloc((u8*) fstr.data(), fstr.length() + 1);

  kgmShader* shader = new kgmShader(gc);

  shader->m_shader = gc->gcGenShader(vsource, fsource);

  if(!shader->m_shader)
  {
    shader->release();

    shader = null;
  }

  return shader;
}

kgmShader* kgmGameTools::genShader(kgmIGC* gc, kgmArray<u8>& vsrc, kgmArray<u8>& fsrc)
{
  if(!gc)
    return null;

  kgmShader* shader = new kgmShader(gc);

  shader->m_shader = gc->gcGenShader(vsrc, fsrc);

  if(!shader->m_shader)
  {
    delete shader;

    shader = null;
  }

  return shader;
}

/*kgmShader* kgmGameTools::genShader(kgmIGC* gc, kgmXml& xml){
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

  if(!shader->m_shader)
  {
    delete shader;
    shader = null;
  }

  return shader;
}*/

kgmMaterial* kgmGameTools::genMaterial(kgmArray<u8>& m){
  kgmMaterial* mtl = 0;
  char* str = 0;
  char* key = 0;
  char* val = 0;
  float farr[4];

  if(m.empty())
    return 0;

  mtl = new kgmMaterial;
  str = new char[1024];
  key = new char[128];
  val = new char[128];

  kgmArrayStream<u8> am(m);

  while(!am.eof())
  {
    memset(str, 0, 1024);
    memset(key, 0, 128);
    memset(val, 0, 128);

    am.reads((u8*)str, 1024, (u8*)"\n", 1);
    sscanf(str, "%s %s", key, val);

    if(!strcmp(key, "color")){
      sscanf(str, "%s %f %f %f %f", key, &farr[0], &farr[1], &farr[2], &farr[3]);
      mtl->m_color = kgmMaterial::Color(farr[0], farr[1], farr[2], farr[3]);
    }
    if(!strcmp(key, "map_texture")){
      mtl->setTexColor(kgmGameApp::gameApp()->game()->getResources()->getTexture(val));
    }
    //if(!strcmp(key, "shader")){
    //  mtl->setShader(kgmIGame::getGame()->getResources()->getShader(val));
    //}
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
    /*else if(id == "Alpha")
    {
      if(mnode->node(i)->attribute("value", val))
      {
        float a;

        sscanf(val, "%f", &a);

        if(a < 1.0f)
        {
          mtl->alpha(true);
        }
      }
    }*/
    else if(id == "Cull")
    {
      mtl->cull(true);
    }
    else if(id == "map_color")
    {
      mnode->node(i)->attribute("value", val);
      mtl->setTexColor(kgmGameApp::gameApp()->game()->getResources()->getTexture(val));
    }
    else if(id == "map_normal")
    {
      mnode->node(i)->attribute("value", val);
      mtl->setTexNormal(kgmGameApp::gameApp()->game()->getResources()->getTexture(val));
    }
    else if(id == "map_specular")
    {
      mnode->node(i)->attribute("value", val);
      mtl->setTexSpecular(kgmGameApp::gameApp()->game()->getResources()->getTexture(val));
    }
    else if(id == "Shader")
    {
      mnode->node(i)->attribute("value", val);
      //mtl->setShader(kgmIGame::getGame()->getResources()->getShader(val));
    }
  }

  return mtl;
}


//SKELETON
kgmSkeleton* kgmGameTools::genSkeleton(kgmArray<u8>& m)
{
  kgmSkeleton* skel;
  char* str = 0;
  char* key = 0;
  char* val = 0;

  if(m.empty())
    return 0;

  skel = new kgmSkeleton;
  str = new char[1024];
  key = new char[128];
  val = new char[128];

  kgmArrayStream<u8> am(m);

  while(!am.eof()){
    memset(str, 0, 1024);
    memset(key, 0, 128);
    memset(val, 0, 128);

    if(0 == am.reads((u8*)str, 1024, (u8*)"\n", 1))
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

      if(0 == am.reads((u8*)str, 1024, (u8*)"\n", 1))
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

  if(skel->m_joints.size() < 1)
  {
    delete skel;

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


  if(skel->m_joints.size() < 1)
  {
    delete skel;

    skel = 0;
  }

  skel->update();

  return skel;
}

//ANIMATION
kgmAnimation* kgmGameTools::genAnimation(kgmArray<u8>& m)
{
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

  kgmArrayStream<u8> am(m);

  while(!am.eof()){
    memset(str, 0, 1024);
    memset(key, 0, 512);

    if(0 == am.reads((u8*)str, 512, (u8*)"\n", 1))
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

  if(anim->count() < 1)
  {
    delete anim;

    anim = 0;
  }

  delete [] str;
  delete [] key;

  return anim;
}

kgmAnimation* kgmGameTools::genAnimation(kgmXml& x)
{
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

  if(anim->count() < 1)
  {
    delete anim;

    anim = 0;
  }

  return anim;
}


//MESHES
kgmMesh* kgmGameTools::genMesh(kgmArray<u8>& mm){
  kgmList<kgmMaterial*> materials;
  kgmMesh* m = 0;
  char* key = 0;
  char* val = 0;
  char* str = 0;

  if(mm.empty())
    return null;

  key = new char[128];
  val = new char[128];
  str = new char[512];

  kgmArrayStream<u8> am(mm);

  while(!am.eof()) {
    memset(key, 0, 128);
    memset(val, 0, 128);
    memset(str, 0, 512);
    am.reads((u8*)str, 512, (u8*)"\n", 1);
    sscanf(str, "%s", key);

    if(!strcmp(key, "Mesh"))
    {
      sscanf(str, "%s %s", key, val);

      if(m)
        break;

      m = new kgmMesh();

      strcpy(m->m_id, val);
    }

    if(!strcmp(key, "Material") && m)
    {
      sscanf(str, "%s %s", key, val);

      if(!strcmp(key, "name") && strlen(val))
        m->setMtlId(val);
    }

    if(!strcmp(key, "Vertices"))
    {
      u32 count = 0;
      u32 maps  = 0;
      kgmMesh::Vertex_P_N_T2*  v = 0;
      sscanf(str, "%s %i %i", key, &count, &maps);
      v = (kgmMesh::Vertex_P_N_T2*)m->vAlloc(count, kgmMesh::FVF_P_N_T2);

      for(u32 i = 0; i < count; i++)
      {
        am.reads((u8*)str, 512, (u8*)"\n", 1);
        sscanf(str, "%f %f %f %f %f %f %f %f",
        &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
        &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
        &v[i].uv[0].x, &v[i].uv[0].y);
      }
    }

    if(!strcmp(key, "Faces"))
    {
      u32 count = 0;
      kgmMesh::Face_16*   f = 0;
      kgmArray<u16> fmaps;
      sscanf(str, "%s %i", key, &count);
      f = (kgmMesh::Face_16*)m->fAlloc(count);
      fmaps.realloc(count);

      for(u32 i = 0; i < count; i++)
      {
        u32 fi[3];
        am.reads((u8*)str, 512, (u8*)"\n", 1);
        sscanf(str, "%ud %ud %ud", &fi[0], &fi[1], &fi[2]);
        f[i].a = fi[0];
        f[i].b = fi[1];
        f[i].c = fi[2];
      }
    }

    if(!strcmp(key, "Skin"))
    {
      u32 count = 0;
      sscanf(str, "%s %i", key, &count);
      kgmMesh::Vertex_P_N_T2* v = new kgmMesh::Vertex_P_N_T2[ m->vcount() ];
      memcpy(v, m->vertices(), m->vcount() * m->vsize());
      kgmMesh::Vertex_P_N_T_BW_BI* s = (kgmMesh::Vertex_P_N_T_BW_BI*)m->vAlloc(count,kgmMesh::FVF_P_N_T_BW_BI);

      for(u32 i = 0; i < count; i++)
      {
        u32  nw = 0;
        u32  bi[4] = {0};
        float bw[4] = {0.0f};
        am.reads((u8*)str, 512, (u8*)"\n", 1);
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
        s[i].bi[3] = bi[3];
        s[i].pos = v[i].pos;
        s[i].nor = v[i].nor;
        s[i].uv = vec2(v[i].uv[0].x, v[i].uv[0].y);
      }

      delete [] v;
    }
  }

  for(int i = 0; i < materials.size(); i++)
    delete materials[i];

  materials.clear();

  delete [] key;
  delete [] val;
  delete [] str;

  return m;
}

kgmMesh* kgmGameTools::genMesh(kgmXml& x)
{
  kgmList<kgmMaterial*> materials;

  kgmMesh* m = 0;

  kgmXml::Node* mnode = 0;

  if(!x.m_node || !(mnode = x.m_node->node("MeshData")))
    return null;

  m = new kgmMesh();

  mnode->attribute("name", m->m_id);

  for(int i = 0; i < mnode->nodes(); i++)
  {
    kgmString id, val, data;

    if(mnode->node(i))
      mnode->node(i)->id(id);
    else
      break;

    if(id == "Material")
    {
      mnode->attribute("name", val);

      m->setMtlId(val);
    }

    if(id == "Vertices")
    {
      u32 count = 0;
      kgmMesh::Vertex_P_N_T*  v = 0;

      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      v = (kgmMesh::Vertex_P_N_T*)m->vAlloc(count, kgmMesh::FVF_P_N_T);
      mnode->node(i)->data(data);

      char* p = data.data();

      for(u32 i = 0; i < count; i++)
      {
        u32 rd;

        sscanf(p, "%f %f %f %f %f %f %f %f %n",
        &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
        &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
        &v[i].uv.x, &v[i].uv.y, &rd);
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

      for(u32 i = 0; i < count; i++)
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

      kgmMesh::Vertex_P_N_T* v = new kgmMesh::Vertex_P_N_T[ m->vcount() ];

      memcpy(v, m->vertices(), m->vcount() * m->vsize());

      kgmMesh::Vertex_P_N_T_BW_BI* s = (kgmMesh::Vertex_P_N_T_BW_BI*)m->vAlloc(count,kgmMesh::FVF_P_N_T_BW_BI);

      mnode->node(i)->data(data);

      char* p = data.data();

      for(u32 i = 0; i < count; i++)
      {
        u32   rd = 0;
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
    delete materials[i];

  materials.clear();

  m->rebuild();

  return m;
}

/*kgmGpuMesh* kgmGameTools::toGpuMesh(kgmIGC* gc, kgmMesh* m)
{
  kgmGpuMesh* gmesh = null;

  if (!gc || !m)
    return null;

  gchandle h = gc->gcGenVertexBuffer(m->vertices(), m->vcount() * m->vsize(),
                                     m->faces(), m->fsize() * m->fcount());

  if (h)
  {
    gmesh = new kgmGpuMesh(h, m->vcount(), m->fcount(), m->vsize(), m->fsize(),
                          m->fvf(), m->fff(), (u32) m->rtype(), m->bound());
  }

  return gmesh;
}*/

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

        for(u32 k = 0; k < count; k++)
        {
          sscanf(pdata, "%f %f %f%n", &v.x, &v.y, &v.z, &n);
          (pdata) += (u32)n;
          pol[k] = v;
        }

        for(u32 k = 2; k < count; k++)
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

        for(u32 k = 0; k < count; k++)
        {
          sscanf(pdata, "%f %f %f%n", &v.x, &v.y, &v.z, &n);
          (pdata) += (u32)n;
          pol[k] = v;
        }

        for(u32 k = 2; k < count; k++)
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

    for(u32 i = 0; i < m.fcount(); i++)
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

    for(u32 i = 0; i < m.vcount(); i+=3)
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

  kgmArray<u8> m;

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
      else if(n->m_name == "Image")
      {
        n->attribute("name", attr);
        gs->gui_image = rc->getTexture(attr);
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
      }
    }

    return gs;
  }

  return null;

#undef KGM_STRHEX_TO_INT
}

//SOUNDS
kgmSound* kgmGameTools::genSound(kgmIAudio* au, kgmArray<u8>& m){
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

kgmGui* kgmGameTools::genGui(kgmGameScript* gs, kgmXml &xml)
{
  kgmGui *base = null, *gui = null;

  kgmGuiMenu::Item * item = null;

  while(kgmXml::XmlState xstate = xml.next())
  {
    kgmString id, value, handler;

    u32 x = 0, y = 0, w = 1, h = 1;

    if(xstate == kgmXml::XML_ERROR || xstate == kgmXml::XML_FINISH)
    {
      return base;
    }
    else if(xstate == kgmXml::XML_TAG_OPEN)
    {
      id = xml.m_tagName;

      if(!strncmp("kgmGui", id.data(), 6))
      {
        xml.attribute("x", value);
        x = kgmConvert::toInteger(value);
        xml.attribute("y", value);
        y = kgmConvert::toInteger(value);
        xml.attribute("w", value);
        w = kgmConvert::toInteger(value);
        xml.attribute("h", value);
        h = kgmConvert::toInteger(value);
        xml.attribute("handler", handler);
      }

      if(id == "kgmGuiLayout")
      {
        gui = new kgmGuiLayout(gui, x, y, w, h);
      }
      else if(id == "kgmGuiTab")
      {
        gui = new kgmGuiTab(gui, x, y, w, h);
      }
      else if(id == "kgmGuiMenu")
      {
        gui = new kgmGuiMenu(gui);

        gs->setSlot(gui, handler);
      }
      else if(id == "kgmGuiList")
      {
        gui = new kgmGuiList(gui, x, y, w, h);

        gs->setSlot(gui, handler);
      }
      else if(id == "kgmGuiFrame")
      {
        xml.attribute("title", value);

        gui = new kgmGuiFrame(value, x, y, w, h);
      }
      else if(id == "MenuItem")
      {
        xml.attribute("title", value);

        if (xml.hasattr("id"))
        {
          kgmString oid;

          xml.attribute("id", oid);

          ((kgmGuiMenu*)gui)->add(kgmConvert::toInteger(oid), value);
        }
        else
        {
          item = ((kgmGuiMenu*)gui)->add(value);
        }
      }

      if (!base)
        base = gui;
    }
    else if(xstate == kgmXml::XML_TAG_CLOSE)
    {
      bool isgui = false;

      kgmString sid;

      id = xml.m_tagName;

      if(!strncmp("kgmGui", id.data(), 6))
      {
        if (xml.attributes())
        {
          xml.attribute("x", value);
          x = kgmConvert::toInteger(value);
          xml.attribute("y", value);
          y = kgmConvert::toInteger(value);
          xml.attribute("w", value);
          w = kgmConvert::toInteger(value);
          xml.attribute("h", value);
          h = kgmConvert::toInteger(value);
          xml.attribute("handler", handler);
          xml.attribute("id", sid);
        }

        isgui = true;
      }

      if(id == "kgmGuiText")
      {
        gui = new kgmGuiText(gui, x, y, w, h);

        gs->setSlot(gui, handler);

        kgmString text;

        xml.attribute("text", text);

        if (text.length() > 0)
          ((kgmGuiText*) gui)->setText(text);
      }
      else if (id == "kgmGuiButton")
      {
        gui = new kgmGuiButton(gui, x, y, w, h);

        gs->setSlot(gui, handler);

        xml.attribute("text", value);

        gui->setText(value);
      }
      else if(id == "kgmGuiProgress")
      {
        gui = new kgmGuiProgress(gui, x, y, w, h);
      }
      else if (id == "ListItem")
      {
        xml.attribute("value", value);

        ((kgmGuiList*)gui)->addItem(value);
      }
      else if (id == "MenuItem")
      {
        kgmString oid;

        if (xml.hasattr("title") && xml.hasattr("id"))
        {
          xml.attribute("title", value);
          xml.attribute("id", oid);

          if (item && oid.length() > 0)
            item->add(kgmConvert::toInteger(oid), value);
        }
      }

      if (sid.length() > 0 && gui)
        gui->setSid(sid);

      if (isgui)
        gui = gui->getParent();
    }
  }

  return base;
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
      kgmMesh::Vertex_P_N_T*  v = 0;

      mnode->node(i)->attribute("length", val);
      sscanf(val.data(), "%i", &count);
      v = (kgmMesh::Vertex_P_N_T*)m->vAlloc(count, kgmMesh::FVF_P_N_T);
      mnode->node(i)->data(data);
      char* p = data.data();

      for(u32 i = 0; i < count; i++)
      {
        u32 rd;
        sscanf(p, "%f %f %f %f %f %f %f %f %n",
        &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
        &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
        &v[i].uv.x, &v[i].uv.y, &rd);
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

      for(u32 i = 0; i < count; i++)
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
      kgmMesh::Vertex_P_N_T* v = new kgmMesh::Vertex_P_N_T[ m->vcount() ];
      memcpy(v, m->vertices(), m->vcount() * m->vsize());
      kgmMesh::Vertex_P_N_T_BW_BI* s = (kgmMesh::Vertex_P_N_T_BW_BI*)m->vAlloc(count,kgmMesh::FVF_P_N_T_BW_BI);
      mnode->node(i)->data(data);

      char* p = data.data();

      for(u32 i = 0; i < count; i++)
      {
        u32   rd = 0;
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
  return null;
}

kgmMaterial* kgmGameTools::parseMaterial(kgmXml::Node& node)
{
  return null;
}

kgmSkeleton* kgmGameTools::parseSkeleton(kgmXml::Node& node)
{
  return null;
}

bool kgmGameTools::initActor(kgmIGame* game, kgmUnit *actor, kgmString id)
{
  kgmArray<u8> mem;

  game->getResources()->getFile(id, mem);

  kgmXml xml(mem);

  mem.clear();

  return kgmGameTools::initActor(game, actor, xml);
}

bool kgmGameTools::initActor(kgmIGame* game, kgmUnit *actor, kgmXml &xml)
{
  if(!game || !actor || !xml.m_node)
    return false;

  kgmXml::Node* a_node = null;

  for(int i = 0; i < xml.m_node->nodes(); i++)
  {
    kgmString id, type;

    xml.m_node->node(i)->id(id);

    if(id == "kgmActor")
    {
      a_node = xml.m_node->node(i);

      xml.m_node->node(i)->attribute("type", type);

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
      f32 mass;

      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f", &mass);

      //actor->body()->m_mass
    }
    else if(id == "Bound")
    {
      float a[3];
      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f%f%f", &a[0], &a[1], &a[2]);

      box3 box;
      box.min = vec3(-0.5 * a[0], -0.5 * a[1], 0.0);
      box.max = vec3( 0.5 * a[0],  0.5 * a[1], a[2]);
      //actor->body()->m_bound.min = vec3(-0.5 * a[0], -0.5 * a[1], 0.0);
      //actor->body()->m_bound.max = vec3( 0.5 * a[0],  0.5 * a[1], a[2]);
    }
    else if(id == "Collision")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "convex")
      {
        kgmArray<u8> mem;
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

                    for(u32 k = 0; k < count; k++)
                    {
                      sscanf(pdata, "%f %f %f%n", &v.x, &v.y, &v.z, &n);
                      (pdata) += (u32)n;
                      pol[k] = v;
                    }

                    for(u32 k = 2; k < count; k++)
                    {
                      vec3 v[3] = {pol[0], pol[k - 1], pol[k]};

                      //actor->body()->addShapeSide(v);
                    }

                    delete [] pol;
                  }
                }
              }
            }
          }
        }

        //if(actor->body()->m_convex.size() > 0)
        //  actor->body()->m_shape = kgmBody::ShapePolyhedron;
      }
    }
    else if(id == "Gravity")
    {
      a_node->node(i)->attribute("value", val);

      /*if(val == "true")
        actor->body()->m_gravity = true;
      else
        actor->body()->m_gravity = false;*/
    }
    else if(id == "Dummies")
    {
      kgmArray<u8> mem;
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
        //actor->visual()->set(msh);
        //actor->visual()->set(mtl);
        //actor->visual()->setAnimation(anm);
        //actor->visual()->setSkeleton(skl);

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

      //actor->add(btn, stat, state, btn1, btn2);
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

      //actor->add(btn, stat, state, btn1, btn2, true);
    }
    else if(id == "State")
    {
      kgmString s;
      kgmIAI::State state;

      a_node->node(i)->attribute("id", state.id);
      a_node->node(i)->attribute("switch", state.switchto);

      a_node->node(i)->attribute("time", s);
      if(s.length() > 0) sscanf(s, "%i", &state.timeout);

      a_node->node(i)->attribute("priority", s);
      if(s.length() > 0) sscanf(s, "%i", &state.priopity);

      for(int j = 0; j < a_node->node(i)->nodes(); j++)
      {
        if(a_node->node(i)->node(j))
          a_node->node(i)->node(j)->id(id);
        else
          break;

        if(id == "Sound")
        {
          a_node->node(i)->node(j)->attribute("value", s);
          state.sound = null;
          state.sound = game->getResources()->getSound(s);
        }
        else if(id == "Animation")
        {
          a_node->node(i)->node(j)->attribute("value", s);
          state.animation = game->getResources()->getAnimation(s);
          a_node->node(i)->node(j)->attribute("start", s);
          if(s.length() > 0) sscanf(s, "%i", &state.fstart);
          a_node->node(i)->node(j)->attribute("end", s);
          if(s.length() > 0) sscanf(s, "%i", &state.fend);
        }
        else if(id == "Option")
        {
          kgmString k, v;
          a_node->node(i)->node(j)->attribute("key", k);
          a_node->node(i)->node(j)->attribute("value", v);
        }
        else if(id == "Action")
        {
          //a_node->node(i)->node(j)->attribute("value", state->action.id);

          for(int k = 0; k < a_node->node(i)->node(j)->nodes(); k++)
          {
            kgmString id;

            if(a_node->node(i)->node(j)->node(k))
              a_node->node(i)->node(j)->node(k)->id(id);
            else
              break;

            if(id == "Variable")
            {
              kgmString name, type, value;

              a_node->node(i)->node(j)->node(k)->attribute("name",  name);
              a_node->node(i)->node(j)->node(k)->attribute("type",  type);
              a_node->node(i)->node(j)->node(k)->attribute("value", value);

              if(!name.empty())
              {
                kgmVariable v;

                if(type == "float")
                {
                  v= kgmVariable(name, (f32) kgmConvert::toDouble(value));
                }
                else if (type == "integer")
                {
                  v= kgmVariable(name, (s32) kgmConvert::toInteger(value));
                }
                else if (type == "boolean")
                {
                  v= kgmVariable(name, (bool) kgmConvert::toBoolean(value));
                }
                else
                {
                  v= kgmVariable(name, value);
                }

                //state->action.variables.add(v);
              }
            }
          }
        }
      }

      //actor->m_states.add(state);
    }
    else
    {
      kgmString val;

      if(a_node->node(i)->attribute("value", val))
      {
        //actor->m_options.set(id, val);
      }
    }
  }

  return true;
}
