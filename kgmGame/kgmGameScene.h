/*
 * kgmGameScene.h
 *
 *  Created on: Mar 17, 2012
 *      Author: Goga
 */

#ifndef KGMGAMESCENE_H_
#define KGMGAMESCENE_H_

#include "../kgmBase/kgmList.h"
#include "../kgmPhysics/kgmPhysics.h"
#include "../kgmGraphics/kgmGraphics.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmGraphics/kgmAnimation.h"

class kgmGameScene
{
public:
  class Node : public kgmNode, public kgmBody
  {
  public:
    Node() :
      kgmNode(), kgmBody()
    {
    }
    Node(mtx4* m) :
      kgmNode(m), kgmBody()
    {
    }
    Node(kgmMesh* m) :
      kgmNode(m), kgmBody()
    {
    }
    Node(kgmLight* m) :
      kgmNode(m), kgmBody()
    {
    }
    Node(kgmCamera* m) :
      kgmNode(m), kgmBody()
    {
    }
    Node(kgmMaterial* m) :
      kgmNode(m), kgmBody()
    {
    }
    Node(kgmAnimation* m) :
      kgmNode(m), kgmBody()
    {
    }
  };

  class Scene
  {
  public:
    kgmCamera* camera;
    kgmList<Node*> nodes;

    Scene()
    {
      camera = 0;
    }

    ~Scene()
    {
    }
  };

public:
  kgmGameScene();
  virtual
  ~kgmGameScene();
};

#endif /* KGMGAMESCENE_H_ */
