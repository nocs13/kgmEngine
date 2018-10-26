// kgmLandscape.h: interface for the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../kgmBase/kgmObject.h"

class kgmPicture;

class kgmTerrain: public kgmObject
{
  KGM_OBJECT(kgmTerrain);

private:
  f32 m_width;
  f32 m_length;
  f32 m_height;

  kgmPicture *m_heighmap;

protected:
  ~kgmTerrain();

public:
  kgmTerrain();

  f32 width() const
  {
    return m_width;
  }

  void width(f32 w)
  {
    m_width = w;
  }

  f32 length() const
  {
    return m_length;
  }

  void length(f32 l)
  {
    m_length = l;
  }

  f32 height() const
  {
    return m_height;
  }

  void height(f32 h)
  {
    m_height = h;
  }

  bool heightmap(kgmPicture* map);
};

