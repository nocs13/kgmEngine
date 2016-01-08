#pragma once
#ifndef KGMGAMEMAP_H
#define KGMGAMEMAP_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmXml.h"

#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmMaterial.h"

#include "kgmActor.h"
#include "kgmSensor.h"
#include "kgmTrigger.h"
#include "kgmUnit.h"

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
    NodeMsh,
    NodeMtl,
    NodeLgt,
    NodeObj,
    NodeAct,
    NodeSns,
    NodeTrg
  };

  struct Node
  {
    NodeType typ = NodeNone;

    kgmObject*   obj = null;
    kgmMaterial* mtl = null;

    vec3       pos;
    vec3       rot;
    box3       bnd;

    kgmString  nam;
    kgmString  lnk;
    kgmString  shd;
    kgmString  shp;
    kgmString  ini;

    bool       col;  //collision type
    bool       hdn;  // node is hidden
    bool       lck;  //locked. for editor only

    Node()
    {
      typ = NodeNone;
      obj = null;
      mtl = null;
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

public:
  kgmGameMap(kgmIGame* g, OpenType ot = OpenRead);
  ~kgmGameMap(){}

  void close();

  bool open(kgmXml& xml);
  bool open(kgmMemory<u8>& mem);

  bool addLight(Node n);
  bool addCamera(Node n);
  bool addVisual(Node n);
  bool addUnit(Node n);
  bool addActor(Node n);
  bool addEffect(Node n);
  bool addSensor(Node n);
  bool addTrigger(Node n);
  bool addObstacle(Node n);


  Node next();

private:
  void addCollision(kgmXml::Node& xnode, Node& node);
  void addCollision(kgmXml::Node& xnode, bool col);

  void addName(kgmXml::Node& xnode, kgmString& name);
  void addLocked(kgmXml::Node& xnode, bool lock);
  void addPosition(kgmXml::Node& xnode, vec3& pos);
  void addRotation(kgmXml::Node& xnode, vec3& rot);
  void addQuaternion(kgmXml::Node& xnode, quat& qt);
  void addTriangle(kgmXml::Node& xnode, triangle3& tr);
  void addParameter(kgmXml::Node& xnode, kgmVariable& param);
  void addParameters(kgmXml::Node& xnode, kgmList<kgmVariable>& params);
};

#endif // KGMGAMEMAP_H
