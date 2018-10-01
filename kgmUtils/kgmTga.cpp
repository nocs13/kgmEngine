#include "kgmTga.h"

#define MAX_RES (12 * 1000)

kgmTga::kgmTga()
{
  memset(&header, 0, sizeof(Header));
}

kgmTga::~kgmTga()
{

}

bool kgmTga::create(kgmMemory<u8>& m)
{
  u8 *pm = m.data();

  if (!pm || m.length() < sizeof(Header))
  {
    return false;
  }

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

  return true;
}

kgmPicture* kgmTga::toPicture()
{

}
