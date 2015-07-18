#include "kgmIGame.h"
#include "kgmUnit.h"

kgmBody* kgmIGame::Node::getBody()
{
  if(object && object->isType(kgmUnit::Class))
    return ((kgmUnit*)object)->getBody();

  return null;
}

kgmUnit* kgmIGame::Node::getUnit()
{
  if(object && object->isType(kgmUnit::Class))
    return (kgmUnit*)object;

  return null;
}

kgmLight* kgmIGame::Node::getLight()
{
  if(object && object->isType(kgmLight::Class))
    return (kgmLight*)object;

  return null;
}

kgmVisual* kgmIGame::Node::getVisual()
{
  if(object && object->isType(kgmUnit::Class))
    return ((kgmUnit*)object)->getVisual();
  else if(object && object->isType(kgmVisual::Class))
    return (kgmVisual*)object;

  return null;
}

