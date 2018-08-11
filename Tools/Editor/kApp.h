#ifndef KAPP_H
#define KAPP_H
#include "../../kgmGame/kgmGameApp.h"
#include "kEditor.h"

class kApp : public kgmGameApp
{
  kEditor* editor;
public:
  kApp();

  void main();
};
#endif // KAPP_H
