#include "kgmIcon.h"
#include "kgmTexture.h"

kgmIcon::kgmIcon(const kgmIcon& icn)
{
  icon = null;
  width = icn.width;
  height = icn.height;
  position = icn.position;
  icon = icn.icon;

  if (icn.icon) 
    icn.icon->assign((kgmObject**) (&this->icon));
}


