#pragma once 

#include "kgmIGraphics.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMath.h"

class kgmMaterial;

class kgmNode: public kgmIGraphics::INode, public kgmObject 
{
  KGM_OBJECT(kgmNode);

  kgmObject* m_object = null;
  kgmIGraphics::TypeNode m_type;

  kgmMaterial* m_material = null;

  mtx4      m_transform;

  kgmIGraphics::NodeShading  m_shading;
  kgmIGraphics::NodeShader   m_shader;

  public:
    kgmNode(kgmObject* o, kgmIGraphics::TypeNode t)
    {
      m_object = o;

      if (o)
        o->increment();

      m_type = t;
      m_shading = kgmIGraphics::ShadingFull;
      m_shader = kgmIGraphics::ShaderNone;

      m_transform.identity();
    }

    ~kgmNode()
    {
      if (m_object)
        m_object->release();
    }

    void setPos(vec3 pos)
    {
      m_transform.translate(pos);
    }

    kgmIGraphics::TypeNode getNodeType()
    {
      return m_type;
    }

    void* getNodeObject()
    {
      return m_object;
    }

    bool isNodeValid()
    {
      return true;
    }

    vec3 getNodePosition()
    {
      return m_transform.position();
    }

    mtx4 getNodeTransform()
    {
      return m_transform;
    }

    box3 getNodeBound()
    {
      box3 bb;

      return bb;
    }

    void setNodeMaterial(kgmMaterial* m)
    {
      m_material = m;
    }

    kgmMaterial* getNodeMaterial()
    {
      return m_material;
    }

    void setNodeShader(kgmIGraphics::NodeShader s)
    {
      m_shader = s;
    }

    kgmIGraphics::NodeShader  getNodeShader()
    {
      return m_shader;
    }

    void setNodeShading(kgmIGraphics::NodeShading s)
    {
      m_shading = s;
    }

    kgmIGraphics::NodeShading  getNodeShading()
    {
      return m_shading;
    }
};
