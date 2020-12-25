#include "kgmFont.h"

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
