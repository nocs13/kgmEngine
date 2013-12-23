#pragma once
#include "../kgmBase/kgmTypes.h"

/////////
// INTERFACE FOR PICTURE
class kgmPicture: public kgmResource
{
public:
  u32 width, height, bpp, frames;
  void*        pdata;
public:
  kgmPicture()
  {
    width = height = bpp = frames = 0;
    pdata = 0;
  }

  kgmPicture(s32 w, s32 h, s32 b, s32 f, void* d)
  {
    width = w;
    height = h;
    bpp = b;
    frames = f;
    pdata = d;
  }

  virtual ~kgmPicture()
  {
    if(pdata)
      free(pdata);

    pdata = null;
  }

  u32 getWidth()
  {
    return width;
  }

  u32 getHeight()
  {
    return height;
  }

  u32 getBpp()
  {
    return bpp;
  }

  void* getData()
  {
    return pdata;
  }

  bool resample(int nwidth, int nheight)
  {
    u8* pdata = (u8*)this->pdata;

    if(!pdata) return false;

    int cbpp = bpp / 8;
    u8* ndata = (u8*)malloc(nwidth * nheight * cbpp);

    double swidth =  (double)nwidth / (double)width;
    double sheight = (double)nheight / (double)height;

    for(int cy = 0; cy < nheight; cy++)
    {
      for(int cx = 0; cx < nwidth; cx++)
      {
        int pixel = (cy * (nwidth * cbpp)) + (cx * cbpp);
        int nearest =  (((int)(cy / sheight) * (width * cbpp)) + ((int)(cx / swidth) * cbpp) );
        u8 r = (pdata[nearest - 1] + pdata[nearest] + pdata[nearest + 1]) / 3;
        u8 g = (pdata[nearest - 1] + pdata[nearest] + pdata[nearest + 1]) / 3;
        u8 b = (pdata[nearest - 1] + pdata[nearest] + pdata[nearest + 1]) / 3;

        ndata[pixel    ] = r;//((u8*)pdata)[nearest    ];
        ndata[pixel + 1] = g;//((u8*)pdata)[nearest + 1];
        ndata[pixel + 2] = b;//((u8*)pdata)[nearest + 2];

        if(cbpp == 4)  ndata[pixel + 3] = ((u8*)pdata)[nearest + 3];
      }
    }

    free(pdata);
    pdata = ndata;
    width = nwidth;
    height = nheight;

    return true;
  }

  //Build From
  bool fromBmp(void* mem, u32 size){
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
    char*  pm = (char*)mem;

    width = height = bpp = 0;
    pdata = 0;

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
    memcpy(&b_climp, pm, 4);      pm += 4;

    if(memcmp("BM",&f_type,2))
      return false;

    if(!b_width || !b_height)
      return false;

    if(b_btcnt < 8)
      return false;

    if(b_btcnt == 8)
    {
      u32 r_size = b_width * b_height;
      pdata    = malloc(sizeof(u32) * r_size);
      u32 *pal = (u32*)malloc(sizeof(u32) * 256);
      memcpy(pal, pm, 256 * 4);	pm += 256 * 4;

      for(u32 i = 0; i < r_size; i++)
      {
        u8 ind = 0;
        memcpy(&ind, pm, 1);	pm += 1;
        ((u32*)pdata)[i] = pal[ind];
      }

      width = b_width;
      height = b_height;
      bpp = 32;

      free(pal);
      pal = null;

      return true;
    }

    u32 r_size = b_width * b_height * (b_btcnt/8);
    pdata = malloc(sizeof(char) * r_size);
    memcpy(pdata, pm, r_size);//////
    width = b_width;
    height = b_height;
    bpp = b_btcnt;

    return true;
  }

  bool fromTga(void* mem, u32 size){
    u8 idl, cmp, dt, btcnt, dsc;
    u8 clmap[5];
    u16  xorgn, yorgn, w, h;
    char* pm = (char*)mem;

    width = height = bpp = 0;
    pdata = 0;

    memcpy(&idl, pm, 1);	pm += 1;
    memcpy(&cmp, pm, 1);	pm += 1;
    memcpy(&dt, pm, 1);	pm += 1;
    memcpy(&clmap, pm, 5);	pm += 5;
    memcpy(&xorgn, pm, 2);	pm += 2;
    memcpy(&yorgn, pm, 2);	pm += 2;
    memcpy(&w, pm, 2);	pm += 2;
    memcpy(&h, pm, 2);	pm += 2;
    memcpy(&btcnt, pm, 1);	pm += 1;
    memcpy(&dsc, pm, 1);	    pm += 1;

    if(dt != 2)
      return false;

    if((btcnt != 24))
      if((btcnt != 32))
        return false;

    u32 r_size = w * h * (btcnt/8);
    pm += idl;
    pdata = malloc(sizeof(char) * r_size);
    memcpy(pdata, pm, r_size);
    u32 k = (u32)((u32*)pdata)[0];
    width = w;
    height = h;
    bpp = btcnt;

    return true;
  }
};
