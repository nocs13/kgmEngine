/*
 * kgmScene.h
 *
 *  Created on: Jul 11, 2010
 *      Author: goga
 */

#ifndef KGMSCENE_H_
#define KGMSCENE_H_

#include "../kgmMain/kgmInterface.h"
#include "kgmNode.h"

class kgmScene: public kgmInterface{
  //Scene
  class Scene{
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

#endif /* KGMSCENE_H_ */
