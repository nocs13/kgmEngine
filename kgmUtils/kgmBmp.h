#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmArray.h"
#include "../kgmBase/kgmResource.h"
#include "../kgmGraphics/kgmPicture.h"

class kgmBmp
{
  KGM_OBJECT(kgmBmp);

  struct Header
  {
    u16 f_type;
    u32 f_size;
    u16 f_res0,
        f_res1;
    u32 f_boffs;
    u32 b_size;
    long b_width,
         b_height;
    u16 b_planes,
        b_btcnt;
    u32 b_compr,
        b_sizeimg;
    long b_xppm,
         b_yppm;
    u32 b_clus,
        b_climp;
  };

  Header header;
  kgmArray<u8> data;

public:
  kgmBmp();
  ~kgmBmp();

  bool create(kgmArray<u8> &m);

  kgmPicture *toPicture();

private:
  void fromUncompressed(u8 *pdata, u32 pixels);
};
