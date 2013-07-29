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
 kgmPicture(){
  width = height = bpp = frames = 0;
  pdata = 0;
 }
 kgmPicture(s32 w, s32 h, s32 b, s32 f, void* d){
  width = w;
  height = h;
  bpp = b;
  frames = f;
  pdata = d;
 }
 virtual ~kgmPicture(){
   if(pdata)
    delete (char*)pdata;
 }
 u32 getWidth(){
  return width;
 }
 u32 getHeight(){
  return height;
 }
 u32 getBpp(){
  return bpp;
 }
 void* getData(){ 
  return pdata;
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

  if(b_btcnt == 8){
   u32 r_size = b_width * b_height;
   pdata = new u32[r_size];
   u32 *pal = new u32[256];
   memcpy(pal, pm, 256 * 4);	pm += 256 * 4;
   for(u32 i = 0; i < r_size; i++){
    u8 ind = 0;
    memcpy(&ind, pm, 1);	pm += 1;
	((u32*)pdata)[i] = pal[ind];
   }
   width = b_width;
   height = b_height;
   bpp = 32;
   delete pal;
   return true;
  }

  u32 r_size = b_width * b_height * (b_btcnt/8);
  pdata = new char[r_size];
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
  pdata = new char[r_size];
  memcpy(pdata, pm, r_size);
  u32 k = (u32)((u32*)pdata)[0];
  width = w;
  height = h;
  bpp = btcnt;
  return true;
 }
};


////////////////////
///// BMP FORMAT 
/*
class kgmBmpPicture: public kgmPicture
{
public:
 kgmBmpPicture(void* mem, u32 size){
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
  memcpy(&b_height, pm, 4);		pm += 4;
  memcpy(&b_planes, pm, 2);		pm += 2;
  memcpy(&b_btcnt, pm, 2);		pm += 2;
  memcpy(&b_compr, pm, 4);		pm += 4;
  memcpy(&b_sizeimg, pm, 4);	pm += 4;
  memcpy(&b_xppm, pm, 4);	pm += 4;
  memcpy(&b_yppm, pm, 4);	pm += 4;
  memcpy(&b_clus, pm, 4);	pm += 4;
  memcpy(&b_climp, pm, 4);  pm += 4;

  if(memcmp("BM",&f_type,2))
   return;

  if(!b_width || !b_height)
   return;

  if(b_btcnt < 8)
   return;

  if(b_btcnt == 8){
   u32 r_size = b_width * b_height;
   pdata = new u32[r_size];
   u32 *pal = new u32[256];
   memcpy(pal, pm, 256 * 4);	pm += 256 * 4;
   for(u32 i = 0; i < r_size; i++){
    uchar ind = 0;
    memcpy(&ind, pm, 1);	pm += 1;
	((u32*)pdata)[i] = pal[ind];
   }
   width = b_width;
   height = b_height;
   bpp = 32;
   delete pal;
   return;
  }
  

  u32 r_size = b_width * b_height * (b_btcnt/8);
  pdata = new char[r_size];
  memcpy(pdata, pm, r_size);//////
  width = b_width;
  height = b_height;
  bpp = b_btcnt;

  return;
  u32* p1 = (u32*)pdata;
  u32* p2 = (u32*)((u32*)pdata + r_size - 4);
  while(p1 < p2){
   u32 a = *p1;
//   *p1 = *p2;
//   *p2 = a;
   p1++;
   p2--;
  }
 }
 virtual ~kgmBmpPicture(){
  if(pdata)
	  delete pdata;
 }
};

////////////
///// TGA FORMAT
class kgmTgaPicture: public kgmPicture
{
public:
 kgmTgaPicture(void* mem, u32 size){
  uchar idl, cmp, dt, btcnt, dsc;
  uchar clmap[5];
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
 	  return;

  if((btcnt != 24))
    if((btcnt != 32))
	  return;

  u32 r_size = w * h * (btcnt/8);
  pm += idl;
  pdata = new char[r_size];
  memcpy(pdata, pm, r_size);
  u32 k = (u32)((u32*)pdata)[0];
  width = w;
  height = h;
  bpp = btcnt;
 }
 virtual ~kgmTgaPicture(){
  if(pdata)
	  delete pdata;
 }
};
*/
