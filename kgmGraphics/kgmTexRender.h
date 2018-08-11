#ifndef KGMTEXRENDER_H
#define KGMTEXRENDER_H

#include "kgmTexture.h"

class kgmTexRender : public kgmTexture
{
  KGM_OBJECT(kgmTexRender)

  void* m_target = null;
public:
  kgmTexRender();
  kgmTexRender(void* tar, void* tex, u32 type = 0);
  ~kgmTexRender();

  void* target() const
  {
    return m_target;
  }
};

#endif // KGMTEXRENDER_H
