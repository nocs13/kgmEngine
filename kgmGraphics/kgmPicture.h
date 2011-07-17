#pragma once

/////////
// INTERFACE FOR PICTURE
class kgmPicture: public kgmResource
{
public:
 unsigned int width, height, bpp, frames;
 void*        pdata;
public:
 kgmPicture(){
  width = height = bpp = frames = 0;
  pdata = 0;
 }
 kgmPicture(int w, int h, int b, int f, void* d){
  width = w;
  height = h;
  bpp = b;
  frames = f;
  pdata = d;
 }
 virtual ~kgmPicture(){
  release();
 }
 void release(){
  if(pdata)
   delete (char*)pdata;
 }
 uint getWidth(){ 
  return width;
 }
 uint getHeight(){ 
  return height;
 }
 uint getBpp(){ 
  return bpp;
 }
 void* getData(){ 
  return pdata;
 }

 //Build From
 bool fromBmp(void* mem, uint size){
  ushort f_type;
  uint   f_size;
  ushort f_res0,f_res1;
  uint   f_boffs;
  uint   b_size;
  long   b_width,b_height;
  ushort b_planes,b_btcnt;
  uint   b_compr,b_sizeimg;
  long   b_xppm,b_yppm;
  uint   b_clus,b_climp;
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
   return false;

  if(!b_width || !b_height)
   return false;

  if(b_btcnt < 8)
   return false;

  if(b_btcnt == 8){
   uint r_size = b_width * b_height;
   pdata = new uint[r_size];
   uint *pal = new uint[256];
   memcpy(pal, pm, 256 * 4);	pm += 256 * 4;
   for(uint i = 0; i < r_size; i++){
    uchar ind = 0;
    memcpy(&ind, pm, 1);	pm += 1;
	((uint*)pdata)[i] = pal[ind];
   }
   width = b_width;
   height = b_height;
   bpp = 32;
   delete pal;
   return true;
  }

  uint r_size = b_width * b_height * (b_btcnt/8);
  pdata = new char[r_size];
  memcpy(pdata, pm, r_size);//////
  width = b_width;
  height = b_height;
  bpp = b_btcnt;
  return true;
 }

 bool fromTga(void* mem, uint size){
  uchar idl, cmp, dt, btcnt, dsc;
  uchar clmap[5];
  ushort  xorgn, yorgn, w, h;
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

  uint r_size = w * h * (btcnt/8);
  pm += idl;
  pdata = new char[r_size];
  memcpy(pdata, pm, r_size);
  uint k = (uint)((uint*)pdata)[0];
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
 kgmBmpPicture(void* mem, uint size){
  ushort f_type;
  uint   f_size;
  ushort f_res0,f_res1;
  uint   f_boffs;
  uint   b_size;
  long   b_width,b_height;
  ushort b_planes,b_btcnt;
  uint   b_compr,b_sizeimg;
  long   b_xppm,b_yppm;
  uint   b_clus,b_climp;
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
   uint r_size = b_width * b_height;
   pdata = new uint[r_size];
   uint *pal = new uint[256];
   memcpy(pal, pm, 256 * 4);	pm += 256 * 4;
   for(uint i = 0; i < r_size; i++){
    uchar ind = 0;
    memcpy(&ind, pm, 1);	pm += 1;
	((uint*)pdata)[i] = pal[ind];
   }
   width = b_width;
   height = b_height;
   bpp = 32;
   delete pal;
   return;
  }
  

  uint r_size = b_width * b_height * (b_btcnt/8);
  pdata = new char[r_size];
  memcpy(pdata, pm, r_size);//////
  width = b_width;
  height = b_height;
  bpp = b_btcnt;

  return;
  uint* p1 = (uint*)pdata;
  uint* p2 = (uint*)((uint*)pdata + r_size - 4);
  while(p1 < p2){
   uint a = *p1;
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
 kgmTgaPicture(void* mem, uint size){
  uchar idl, cmp, dt, btcnt, dsc;
  uchar clmap[5];
  ushort  xorgn, yorgn, w, h;
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

  uint r_size = w * h * (btcnt/8);
  pm += idl;
  pdata = new char[r_size];
  memcpy(pdata, pm, r_size);
  uint k = (uint)((uint*)pdata)[0];
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
