#include "kgmIGame.h"
#include "kgmUnit.h"

kgm_ptr<kgmBody> kgmIGame::Node::getBody()
{
  kgm_ptr<kgmBody> body;

  if(object && object->isType(kgmUnit::Class))
  {
    body = kgm_ptr<kgmBody>(kgm_ptr_cast<kgmUnit, kgmObject>(object)->getBody());
  }

  return body;
}

kgm_ptr<kgmUnit> kgmIGame::Node::getUnit()
{
  if(object && object->isType(kgmUnit::Class))
    return kgm_ptr<kgmUnit>(kgm_ptr_cast<kgmUnit, kgmObject>(object));

  return null;
}

kgm_ptr<kgmLight> kgmIGame::Node::getLight()
{
  if(object && object->isType(kgmLight::Class))
    return kgm_ptr<kgmLight>(kgm_ptr_cast<kgmLight, kgmObject>(object));

  return null;
}

kgm_ptr<kgmVisual> kgmIGame::Node::getVisual()
{
  if(object && object->isType(kgmUnit::Class))
    return kgm_ptr<kgmVisual>(kgm_ptr_cast<kgmUnit, kgmObject>(object)->getVisual());
  else if(object && object->isType(kgmVisual::Class))
    return kgm_ptr<kgmVisual>(kgm_ptr_cast<kgmVisual, kgmObject>(object));

  return null;
}

vec3 kgmIGame::Node::getPosition()
{
  vec3 v(0, 0, 0);

  switch(type)
  {
  case NodeUnt:
  case NodeAct:
  case NodeSns:
  case NodeTrg:
    return kgm_ptr_cast<kgmUnit, kgmObject>(object)->getPosition();
  case NodeLgt:
    return kgm_ptr_cast<kgmLight, kgmObject>(object)->getPosition();
  case NodeVis:
    return kgm_ptr_cast<kgmVisual, kgmObject>(object)->getTransform() * v;
  }

  return v;
}

void kgmIGame::Node::setPosition(vec3 pos)
{
  switch(type)
  {
  case NodeUnt:
  case NodeAct:
  case NodeSns:
  case NodeTrg:
    kgm_ptr_cast<kgmUnit, kgmObject>(object)->setPosition(pos);
  case NodeLgt:
    kgm_ptr_cast<kgmLight, kgmObject>(object)->setPosition(pos);
  case NodeVis:
    kgm_ptr_cast<kgmVisual, kgmObject>(object)->getTransform().translate(pos);
  }
}

vec3 kgmIGame::Node::getRotation()
{
  vec3 v(0, 0, 0);

  switch(type)
  {
  case NodeUnt:
  case NodeAct:
  case NodeSns:
  case NodeTrg:
    return kgm_ptr_cast<kgmUnit, kgmObject>(object)->getRotation();
  case NodeLgt:
    return kgm_ptr_cast<kgmLight, kgmObject>(object)->getDirection();
  case NodeVis:
    return v;
  }

  return v;
}

void kgmIGame::Node::setRotation(vec3 rot)
{
  switch(type)
  {
  case NodeUnt:
  case NodeAct:
  case NodeSns:
  case NodeTrg:
    kgm_ptr_cast<kgmUnit, kgmObject>(object)->setRotation(rot);
  case NodeLgt:
    kgm_ptr_cast<kgmLight, kgmObject>(object)->setDirection(rot);
  case NodeVis:
    kgm_ptr_cast<kgmVisual, kgmObject>(object)->getTransform().rotate(rot);
  }
}
