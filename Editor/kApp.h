#ifndef KAPP_H
#define KAPP_H
#include "../kgmSystem/kgmApp.h"
#include "kEditor.h"

class kApp : public kgmApp
{
  kEditor* editor = null;
  bool failed = false;

public:
  kApp();
  ~kApp();

  void main();

  void abort() { }
};
#endif // KAPP_H
