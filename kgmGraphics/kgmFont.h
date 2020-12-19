#pragma once
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmResource.h"
#include "../kgmMath/kgmMath.h"
#include "kgmTexture.h"

#ifdef WIN32
#include <windows.h>
#endif

class kgmFont: public kgmResource
{
  KGM_OBJECT(kgmFont);

  enum FONT_SIZE {
    FONT_SIZE_NORMAL,
    FONT_SIZE_SMALL,
    FONT_SIZE_BIG,
  };
public:
  s32  m_twidth, m_theight, m_fwidth, m_fheight;
  u32 m_rows, m_cols, m_base;

  gchandle m_fonts[3];

public:
  kgmFont()
  {
    m_twidth = m_theight = m_fwidth = m_fheight = 0;
    
    m_rows = m_cols = 0, m_base = 32;
  }
  
  kgmFont(gchandle t_nor = null, gchandle t_big = null, gchandle t_small = null);

  ~kgmFont()
  {
  }

  kgmTexture* texture();
};
