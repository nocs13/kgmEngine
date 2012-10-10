/*
 * kgmGameNode.h
 *
 *  Created on: May 27, 2012
 *      Author: Goga
 */

#ifndef KGMGAMENODE_H_
#define KGMGAMENODE_H_

#include "../kgmGraphics/kgmGraphics.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmBase/kgmList.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmGraphics/kgmAnimation.h"
#include "../kgmPhysics/kgmPhysics.h"

class kgmGameNode: public kgmNode, public kgmBody 
{
 public: 
  kgmString m_name;   
     
  kgmList<kgmGameNode*> m_nodes;

 public:
 kgmGameNode() :
  kgmNode(), kgmBody()
    {
    }
  
 kgmGameNode(mtx4* m) :
  kgmNode(m), kgmBody()
    {
    }

 kgmGameNode(kgmMesh* m) :
  kgmNode(m), kgmBody()
    {
    }

 kgmGameNode(kgmLight* m) :
  kgmNode(m), kgmBody()
    {
    }

 kgmGameNode(kgmCamera* m) :
  kgmNode(m), kgmBody()
    {
    }

 kgmGameNode(kgmMaterial* m) :
  kgmNode(m), kgmBody()
    {
    }

 kgmGameNode(kgmAnimation* m) :
  kgmNode(m), kgmBody()
    {
    }

  virtual ~kgmGameNode()
    {
      for(int i = 0; i < m_nodes.size(); i++)
        {
          if(m_nodes[i])
            m_nodes[i]->release();
        }
      m_nodes.clear();
    }

  virtual int nodes() { return m_nodes.size(); }

  virtual kgmNode* node(int i) { return (i < m_nodes.size()) ? (m_nodes[i]) : (0); }
};

#endif /* KGMGAMENODE_H_ */
