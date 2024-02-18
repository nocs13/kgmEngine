#include "kgmBmp.h"

kgmBmp::kgmBmp()
{
  memset(&header, 0, sizeof(Header));
}

kgmBmp::~kgmBmp()
{

}

bool kgmBmp::create(kgmArray<u8>& m)
{
  u8 *pm = m.data();

  if (!pm || m.length() < sizeof(Header))
  {
    return false;
  }

  data.clear();

  memcpy(&header.f_type, pm, 2);	pm += 2;
  memcpy(&header.f_size, pm, 4);	pm += 4;
  memcpy(&header.f_res0, pm, 2);	pm += 2;
  memcpy(&header.f_res1, pm, 2);	pm += 2;
  memcpy(&header.f_boffs, pm, 4);	pm += 4;
  memcpy(&header.b_size, pm, 4);	pm += 4;
  memcpy(&header.b_width, pm, 4);	pm += 4;
  memcpy(&header.b_height, pm, 4);	pm += 4;
  memcpy(&header.b_planes, pm, 2);	pm += 2;
  memcpy(&header.b_btcnt, pm, 2);	pm += 2;
  memcpy(&header.b_compr, pm, 4);	pm += 4;
  memcpy(&header.b_sizeimg, pm, 4);	pm += 4;
  memcpy(&header.b_xppm, pm, 4);	pm += 4;
  memcpy(&header.b_yppm, pm, 4);	pm += 4;
  memcpy(&header.b_clus, pm, 4);	pm += 4;
  memcpy(&header.b_climp, pm, 4);  pm += 4;

  if(memcmp("BM",&header.f_type, 2))
    return false;

  if(!header.b_width || !header.b_height)
    return false;

  if(header.b_btcnt < 8)
    return false;

  if(header.b_btcnt == 8)
  {
    u32 r_size = header.b_width * header.b_height;

    data.alloc(sizeof(u32) * r_size);

    u32 *pal = (u32*)kgm_alloc(sizeof(u32) * 256);

    memcpy(pal, pm, 256 * 4);	pm += 256 * 4;

    for(u32 i = 0; i < r_size; i++)
    {
      u8 ind = 0;
      memcpy(&ind, pm, 1);	pm += 1;
      ((u32*)data.data())[i] = pal[ind];
    }

    kgm_free(pal);

    return true;
  }

  u32 r_size = header.b_width * header.b_height * (header.b_btcnt / 8);
  data.alloc(sizeof(char) * r_size);
  memcpy(data.data(), pm, r_size);

  for(int i = 0; i < (header.b_width * header.b_height); i++)
  {
    char* pt = (char*)(((char*)data.data()) + i * header.b_btcnt / 8);
    char  t  = pt[0];
    pt[0] = pt[2];
    pt[2] = t;
  }

  return true;
}

kgmPicture* kgmBmp::toPicture()
{
  if (data.data() || data.length())
  {
    u8* pdata = (u8*) kgm_alloc(data.length());

    memcpy(pdata, data.data(), data.length());

    return new kgmPicture(header.b_width, header.b_height, header.b_btcnt, 1, pdata);
  }

  return null;
}
