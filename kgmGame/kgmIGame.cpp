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

vec3 kgmIGame::Node::getPosition()
{
  vec3 v(0, 0, 0);

  switch(type)
  {
  case NodeUnt:
  case NodeAct:
  case NodeSns:
  case NodeTrg:
    return ((kgmUnit*)object)->getPosition();
  case NodeLgt:
    return ((kgmLight*)object)->position;
  case NodeVis:
    return ((kgmVisual*)object)->getTransform() * v;
  }

  return vec3(0, 0, 0);
}

void kgmIGame::Node::setPosition(vec3 pos)
{
  switch(type)
  {
  case NodeUnt:
  case NodeAct:
  case NodeSns:
  case NodeTrg:
    ((kgmUnit*)object)->setPosition(pos);
  case NodeLgt:
    ((kgmLight*)object)->position = pos;
  case NodeVis:
    ((kgmVisual*)object)->getTransform().translate(pos);
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
    return ((kgmUnit*)object)->getRotation();
  case NodeLgt:
    return ((kgmLight*)object)->direction;
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
    ((kgmUnit*)object)->setRotation(rot);
  case NodeLgt:
    ((kgmLight*)object)->direction = rot;
  case NodeVis:
    ((kgmVisual*)object)->getTransform().rotate(rot);
  }
}
