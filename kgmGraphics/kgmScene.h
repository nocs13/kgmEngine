#ifndef KGMSCENE_H_
#define KGMSCENE_H_

#include "kgmNode.h"

class kgmScene: public kgmObject
{
  //Scene
  class Scene
  {
  public:
    kgmList<kgmNode*>  nodes;
    kgmList<Scene*>    scenes;
  };

public:
  Scene* m_scene;

public:
  kgmScene();
  virtual ~kgmScene();
};

#endif
