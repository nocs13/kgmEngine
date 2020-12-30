#include "kgmFont.h"

#define KGM_FONT_PIX_SMALL 11
#define KGM_FONT_PIX_LARGE 21

kgmFont::kgmFont(gchandle t_nor, gchandle t_big, gchandle t_small)
{
  m_fonts[FONT_SIZE_NORMAL] = t_nor;
  m_fonts[FONT_SIZE_SMALL]  = t_small;
  m_fonts[FONT_SIZE_BIG]    = t_big;
}

gchandle kgmFont::texture()
{
  return m_fonts[FONT_SIZE_NORMAL];
}

gchandle kgmFont::texture(u32 s)
{
  if (s < KGM_FONT_PIX_SMALL)
    return m_fonts[FONT_SIZE_SMALL];
  else if (s > KGM_FONT_PIX_LARGE)
    return m_fonts[FONT_SIZE_BIG];

  return m_fonts[FONT_SIZE_NORMAL];
}
