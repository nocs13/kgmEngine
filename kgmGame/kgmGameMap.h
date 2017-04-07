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
  bool save(kgmString path);

  kgmUnit* next();

private:
  bool addUnit(kgmUnit* n);
  bool addLight(kgmUnit* n);
  bool addActor(kgmActor* n);
  bool addCamera(kgmUnit* n);
  bool addVisual(kgmUnit* n);
  bool addEffect(kgmEffect* n);
  bool addSensor(kgmSensor* n);
  bool addTrigger(kgmTrigger* n);

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
