#ifndef KAPP_H
#define KAPP_H
#include "../kgmSystem/kgmApp.h"
#include "kEditor.h"

class kApp : public kgmApp
{
  KGM_OBJECT(kApp)
  kEditor* editor = null;
  bool failed = false;

public:
  kApp();
  ~kApp();

  s32  exec(s32, s8**);

  void abort() { }
};
#endif // KAPP_H
