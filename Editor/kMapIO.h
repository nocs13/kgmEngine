#ifndef KMAPIO_H
#define KMAPIO_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmBase/kgmString.h"

class kMapIO : public kgmObject
{
public:
  kMapIO();

  bool open(kgmString);
  bool create(kgmString);

  void close();
};

#endif // KMAPIO_H
