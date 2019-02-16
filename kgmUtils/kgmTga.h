#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmMemory.h"
#include "../kgmBase/kgmResource.h"
#include "../kgmGraphics/kgmPicture.h"

class kgmTga
{
  KGM_OBJECT(kgmTga);

  enum Type
  {
    Null   = 0,
    Map    = 1,
    Rgb    = 2,
    Mono   = 3,
    RleMap = 9,
    RleRgb = 10,
    RleMono = 11,
    //DESC_ALPHA_MASK ((unsigned char)0xF) // number of alpha channel bits
    //DESC_ORG_MASK ((unsigned char)0x30) // origin mask
    //ORG_BOTTOM_LEFT 0x00 // origin mask
    //ORG_BOTTOM_RIGHT 0x10
    //ORG_TOP_LEFT 0x20
    //ORG_TOP_RIGHT 0x30
  };

  struct Header
  {
    u8  idl;    // length of Identifier String.
    u8  cmap;   // 0 = no map
    u8  type;   // image type (see below for values)
    u16 idx;    // index of first color map entry
    u16 ecm;    // number of entries in color map
    u8  scm;    // size of color map entry (15,16,24,32)
    u16 orx;    // x origin of image
    u16 ory;    // y origin of image
    u16 width;  // width of image
    u16 height; // height of image
    u8  bpp;  // pixel size (8,16,24,32)
    u8  desc;   // 4 bits are number of attribute bits per pixel
  };

  Header        header;
  kgmMemory<u8> data;

  u16 ppBytes;

public:
  kgmTga();
  ~kgmTga();

  bool create(kgmMemory<u8>& m);

  kgmPicture* toPicture();

private:
  void fromCompressed(u8* pdata, u32 pixels);
  void fromUncompressed(u8* pdata, u32 pixels);
};
