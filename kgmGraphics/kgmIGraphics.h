#ifndef KGMIGRAPHICS_H
#define KGMIGRAPHICS_H

#include "../kgmBase/kgmString.h"
#include "../kgmMath/kgmBase.h"

class kgmObject;

class kgmIcon;
class kgmText;
class kgmLight;
class kgmVisual;
class kgmCamera;
class kgmTerrain;
class kgmResource;
class kgmMaterial;

class kgmIGraphics
{
public:
  enum TypeNode
  {
    NodeNone,
    NodeMesh,
    NodeText,
    NodeLight,
    NodeSprite,
    NodeVisual,
    NodeCamera,
    NodeSkinned,
    NodeTerrain,
    NodeParticles
  };

  enum NodeShading 
  {
    ShadingNone,
    ShadingFull
  };

  class INode
  {
  public:
    virtual ~INode(){}

    virtual void*      getNodeObject()    = 0;
    virtual TypeNode   getNodeType()      = 0;
    virtual bool       isNodeValid()      = 0;

    virtual box3       getNodeBound()     = 0;
    virtual vec3       getNodePosition()  = 0;
    virtual mtx4       getNodeTransform() = 0;

    virtual void         setNodeMaterial(kgmMaterial*) = 0;
    virtual kgmMaterial* getNodeMaterial()             = 0;

    virtual void         setNodeShading(NodeShading)   = 0;
    virtual NodeShading  getNodeShading()              = 0;
  };

public:
  virtual void clear()  = 0;
  virtual void render() = 0;

  virtual void render_3d() = 0;
  virtual void render_2d() = 0;

  virtual void add(INode*) = 0;
  virtual void terrain(INode*) = 0;

  virtual bool resource(kgmResource*)      = 0;
  virtual kgmResource* resource(kgmString) = 0;

  virtual kgmCamera& camera() = 0;
};

#endif // KGMIGRAPHICS_H
