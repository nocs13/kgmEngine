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
#include "kgmGameObject.h"

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
    NodeType typ;

    kgmObject* obj;

    vec3       pos;
    vec3       rot;
    box3       bnd;

    kgmString  nam;
    kgmString  lnk;
    kgmString  mtl;
    kgmString  shp;

    bool       col;  //collision type
    bool       hdn;  // node is hidden
    bool       lck;  //locked. for editor only
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

  bool addMesh(Node n);
  bool addLight(Node n);
  bool addCamera(Node n);
  bool addMaterial(Node n);
  bool addActor(Node n);
  bool addSensor(Node n);
  bool addTrigger(Node n);
  bool addGameObject(Node n);

  void addCollision(kgmXml::Node& xnode, Node& node);

  Node next();
};

#endif // KGMGAMEMAP_H
