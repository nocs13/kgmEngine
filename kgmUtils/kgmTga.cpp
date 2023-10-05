#include "kgmTga.h"

#define MAX_RES (12 * 1000)

kgmTga::kgmTga()
{
  memset(&header, 0, sizeof(Header));
  ppBytes = 0;
}

kgmTga::~kgmTga()
{

}

bool kgmTga::create(kgmArray<u8>& m)
{
  u8 *pm = m.data();

  if (!pm || m.length() < sizeof(Header))
  {
    return false;
  }

  data.clear();

  memcpy(&header.idl,    pm, 1); pm += 1;
  memcpy(&header.cmap,   pm, 1); pm += 1;
  memcpy(&header.type,   pm, 1); pm += 1;
  memcpy(&header.idx,    pm, 2); pm += 2;
  memcpy(&header.ecm,    pm, 2); pm += 2;
  memcpy(&header.scm,    pm, 1); pm += 1;
  memcpy(&header.orx,    pm, 2); pm += 2;
  memcpy(&header.ory,    pm, 2); pm += 2;
  memcpy(&header.width,  pm, 2); pm += 2;
  memcpy(&header.height, pm, 2); pm += 2;
  memcpy(&header.bpp,    pm, 1); pm += 1;
  memcpy(&header.desc,   pm, 1); pm += 1;

  if ((header.type != Type::Rgb) && (header.type != Type::RleRgb))
  {
    return false;
  }

  if (header.width > MAX_RES || header.height > MAX_RES)
  {
    return false;
  }

  bool compr;

  if(header.type == Type::Rgb)
    compr = false;
  else if (header.type == Type::RleRgb)
    compr = true;
  else
    return false;

  ppBytes = header.bpp / 8;

  if (ppBytes < 3)
    ppBytes = 3;

  u32 size = header.width * header.height * ppBytes;

  data.alloc(size);

  if (compr)
    fromCompressed(pm, header.width * header.height);
  else
    fromUncompressed(pm, header.width * header.height);

  return true;
}

kgmPicture* kgmTga::toPicture()
{
  if (data.data() || data.length())
  {
    u8* pdata = (u8*) kgm_alloc(data.length());

    memcpy(pdata, data.data(), data.length());

    return new kgmPicture(header.width, header.height, header.bpp, 1, pdata);
  }

  return null;
}

void kgmTga::fromCompressed(u8* pdata, u32 pixels)
{
  u32 cpixel = 0, cbyte = 0;

  u8 hchunk = 0;

  u32 index = 0;

  u8 ppbytes = header.bpp / 8;

  u8 rgba[4];

  while(true)
  {
    if (cbyte >= data.length())
      break;

    memcpy(&hchunk, &pdata[index], 1); index++;

    if (hchunk < 128)
    {
      hchunk++;

      for (u32 i = 0; i < hchunk; i++)
      {
        memcpy(rgba, &pdata[index], ppbytes); index += ppbytes;

        switch(ppbytes)
        {
        case 2:
          break;
        case 4:
          data.data()[cbyte + 3] = rgba[3];
        default:
          data.data()[cbyte]     = rgba[2];
          data.data()[cbyte + 1] = rgba[1];
          data.data()[cbyte + 2] = rgba[0];

          cbyte += ppBytes;
        };
      }
    }
    else
    {
      hchunk -= 127;

      memcpy(rgba, &pdata[index], ppbytes); index += ppbytes;

      for (u32 i = 0; i < hchunk; i++)
      {
        switch(ppbytes)
        {
        case 2:
          break;
        case 4:
          data.data()[cbyte + 3] = rgba[3];
        default:
          data.data()[cbyte]     = rgba[2];
          data.data()[cbyte + 1] = rgba[1];
          data.data()[cbyte + 2] = rgba[0];

          cbyte += ppBytes;
        };
      }
    }
  }
}

void kgmTga::fromUncompressed(u8* pdata, u32 pixels)
{
  if (header.bpp == 16)
  {
    for(u32 i = 0; i < pixels; i++)
    {
      data.data()[3 * i + 0] = ( pdata[2 * i + 0] & 0x1f) << 3;
      data.data()[3 * i + 1] = ((pdata[2 * i + 0] & 0xe0) >> 2) | ((pdata[2 * i + 1] & 0x03) << 6);
      data.data()[3 * i + 2] = ( pdata[2 * i + 1] & 0x7c) << 1;
    }
  }
  else if (header.bpp > 16)
  {
    //memcpy(data.data(), pdata, pixels * ppBytes);
    for(u32 i = 0; i < pixels; i++)
    {
      data.data()[4 * i + 0] = pdata[4 * i + 2];
      data.data()[4 * i + 1] = pdata[4 * i + 1];
      data.data()[4 * i + 2] = pdata[4 * i + 0];
      data.data()[4 * i + 3] = pdata[4 * i + 3];
    }
  }
}
