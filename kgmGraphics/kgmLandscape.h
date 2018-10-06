// kgmLandscape.h: interface for the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../kgmBase/kgmObject.h"

class kgmLandscape: public kgmObject
{
  KGM_OBJECT(kgmLandscape);

private:
  f32 m_width;
  f32 m_length;
  f32 m_height;

  kgmPicture *m_heighmap;

protected:
  ~kgmLandscape();

public:
  kgmLandscape();

  f32 width() const
  {
    return m_width;
  }

  void width(f32 w)
  {
    m_width = w;
  }

  f32 height() const
  {
    return m_height;
  }

  void height(f32 h)
  {
    m_height = wh;
  }

  f32 length() const
  {
    return m_length;
  }

  void length(f32 l)
  {
    m_length = l;
  }
};

