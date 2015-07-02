#pragma once
#include "../kgmBase/kgmResource.h"
#include "../kgmMath/kgmMath.h"
#include "kgmTexture.h"

#ifdef WIN32
#include <windows.h>
#endif

class kgmFont: public kgmTexture
{
  KGM_OBJECT(kgmFont);

public:
  int  m_twidth, m_theight,
  m_fwidth, m_fheight;
  u32 m_rows, m_cols, m_base;

protected:
  ~kgmFont()
  {

  }

public:
  kgmFont()
  {
    m_twidth = m_theight =
        m_fwidth = m_fheight = 0;
    m_rows = m_cols = 0, m_base = 32;
  }
};
