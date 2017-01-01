#pragma once
#ifndef KGMGAMEMAP_H
#define KGMGAMEMAP_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmXml.h"

#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmMaterial.h"

#include "kgmUnit.h"
#include "kgmActor.h"
#include "kgmEffect.h"
#include "kgmSensor.h"
#include "kgmTrigger.h"

class kgmIGame;

class kgmGameMap : public kgmObject
{
  KGM_OBJECT(kgmGameMap);
public:
  enum OpenType
  {
    OpenRead,
    OpenWrite
  };

  enum NodeType
  {
    NodeNone,
    NodeCam,
    NodeVis,
    NodeMtl,
    NodeLgt,
    NodeUnt,
    NodeEff,
    NodeAct,
    NodeSns,
    NodeTrg,
    NodeObs
  };

  struct Node
  {
    NodeType typ = NodeNone;

    kgmIObject*  obj = null;

    vec3  pos;
    vec3  rot;
    box3  bnd;

    kgmString  nam;
    kgmString  lnk;
    kgmString  shd;
    kgmString  shp;
    kgmString  ini;
    kgmString  mtl;

    bool  col;  //collision type
    bool  hdn;  // node is hidden
    bool  lck;  //locked. for editor only

    float  fov;

    Node()
    {
      typ = NodeNone;
      obj = null;
    }
  };

private:
  enum DataFormat
  {
    DFBin,
    DFXml
  };

  OpenType        m_type;

  kgmXml*         m_xml;
  kgmMemory<u8>*  m_mem;

  kgmIGame*       m_game;

  kgmList<kgmMaterial*>  m_materials;

public:
  kgmGameMap(kgmIGame* g, OpenType ot = OpenRead);
  ~kgmGameMap()
  {
    m_materials.clear();
  }

  void close();

  bool open(kgmXml& xml);
  bool open(kgmMemory<u8>& mem);

  bool addLight(Node n);
  bool addCamera(Node n);
  bool addVisual(Node n);
  bool addUnit(Node n);
  bool addActor(kgmActor* n);
  bool addEffect(Node n);
  bool addSensor(Node n);
  bool addTrigger(Node n);
  bool addObstacle(Node n);
  bool addMaterial(Node n);

  Node next();

private:
  void addCollision(kgmXml::Node& xnode, Node& node);
  void addCollision(kgmXml::Node& xnode, bool col);

  void addName(kgmXml::Node& xnode, kgmString& name);
  void addLocked(kgmXml::Node& xnode, bool lock);
  void addPosition(kgmXml::Node& xnode, vec3 pos);
  void addRotation(kgmXml::Node& xnode, vec3 rot);
  void addQuaternion(kgmXml::Node& xnode, quat qt);
  void addTriangle(kgmXml::Node& xnode, triangle3& tr);
  void addParameter(kgmXml::Node& xnode, kgmVariable& param);
  void addParameters(kgmXml::Node& xnode, kgmList<kgmVariable>& params);
};

#endif // KGMGAMEMAP_H
