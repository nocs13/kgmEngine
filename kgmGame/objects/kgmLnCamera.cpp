#include "kgmLnCamera.h"
#include "../kgmIGame.h"
#include "../kgmILogic.h"
#include "../kgmActor.h"
#include "../../kgmGraphics/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmLnCamera, kgmSensor);

kgmLnCamera::kgmLnCamera(kgmIGame* g)
:kgmSensor(g)
{
  m_camera = null;
  m_linked = null;

  m_place = vec3(-1, 0, 1);
  m_look  = vec3(0, 0, 0.2);

  kgmVariable var;

  var = kgmVariable("linkto",   kgmString(""), &m_lname);
  m_variables.add(var);
  var = kgmVariable("placeX", 0.0f, &m_place.x);
  m_variables.add(var);
  var = kgmVariable("placeY", 0.0f, &m_place.y);
  m_variables.add(var);
  var = kgmVariable("placeZ", 0.0f, &m_place.z);
  m_variables.add(var);
  var = kgmVariable("lookX", 0.0f, &m_look.x);
  m_variables.add(var);
  var = kgmVariable("lookY", 0.0f, &m_look.y);
  m_variables.add(var);
  var = kgmVariable("lookZ", 0.0f, &m_look.z);
  m_variables.add(var);
}

kgmLnCamera::~kgmLnCamera()
{
//  if(m_linked)
//    m_linked->release();
}

void kgmLnCamera::init()
{
  if(game())
    m_camera = &game()->getGraphics()->camera();
}

void kgmLnCamera::update(u32 ms)
{
  if(!valid() || !m_camera)
    return;

  if(!m_linked)
  {
    m_linked = game()->getLogic()->getPlayer();

    if(!m_linked)
      return;
  }

  if(!m_linked->getBody())
    return;

  vec3 pos = m_linked->getBody()->position();

  vec3 c_pos  = pos + m_place;
  vec3 c_look = pos + m_look;
  vec3 c_ldir = c_look - c_pos;

  c_ldir.normalize();
  m_camera->mPos = c_pos;
  m_camera->mDir = c_ldir;
  m_camera->update();
}

bool kgmLnCamera::linkto(kgmString name)
{
  if(!game() || !game()->getLogic())
    return false;

  kgmActor *a = (kgmActor*)game()->getLogic()->getObjectById(name);

  if(!a)
    return false;

//  if(m_linked)
//    m_linked->release();

//  a->increment();

  m_linked = a;
  m_lname = name;

  return true;
}
