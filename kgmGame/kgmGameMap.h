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
class kgmGameBase;

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
  kgmArray<u8>*   m_mem;

  kgmGameBase*    m_game;

public:
  kgmGameMap(kgmGameBase* g, OpenType ot = OpenRead);
  ~kgmGameMap()
  {
  }

  void close();

  bool open(kgmXml& xml);
  bool open(kgmArray<u8>& mem);
  bool save(kgmString path);

  kgmUnit* next();

private:
  bool addUnit(kgmUnit* n);
  bool addMesh(kgmUnit* n);
  bool addLight(kgmUnit* n);
  bool addActor(kgmActor* n);
  bool addCamera(kgmUnit* n);
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

  //

  kgmMesh*     getGameMesh(kgmString id);
  kgmMaterial* getGameMaterial(kgmString id);

  void xmlAttr(kgmXml* node, kgmString id, kgmString& val)
  {
    if(node->hasattr(id))
      node->attribute(id, val);
  }

  void xmlAttr(kgmXml* node, kgmString id, s32& val)
  {
    kgmString v;

    if(node->hasattr(id))
    {
      node->attribute(id, v);
      val = atoi(v.data());
    }
  }

  void xmlAttr(kgmXml* node, kgmString id, f32& val)
  {
    kgmString v;

    if(node->hasattr(id))
    {
      node->attribute(id, v);
      val = atof(v.data());
    }
  }

  void xmlAttr(kgmXml* node, kgmString id, vec3& val)
  {
    kgmString v;
    float     a[3];

    if(node->hasattr(id))
    {
      node->attribute(id, v);
      sscanf(v.data(), "%f %f %f", &a[0], &a[1], &a[2]);
      val = vec3(a[0], a[1], a[2]);
    }
  }

  void xmlAttr(kgmXml* node, kgmString id, vec4& val)
  {
    kgmString v;
    float     a[4];

    if(node->hasattr(id))
    {
      node->attribute(id, v);
      sscanf(v.data(), "%f %f %f %f", &a[0], &a[1], &a[2], &a[3]);
      val = vec4(a[0], a[1], a[2], a[3]);
    }
  }

  void xmlAttr(kgmXml* node, kgmString id, quat& val)
  {
    kgmString v;
    float     a[4];

    if(node->hasattr(id))
    {
      node->attribute(id, v);
      sscanf(v.data(), "%f %f %f %f", &a[0], &a[1], &a[2], &a[3]);
      val = quat(a[0], a[1], a[2], a[3]);
    }
  }
};

#endif // KGMGAMEMAP_H
