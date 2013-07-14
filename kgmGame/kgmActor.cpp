#include "kgmActor.h"
KGMOBJECT_IMPLEMENT(kgmActor, kgmGameObject);

kgmActor::kgmActor()
{
  vec3 tv(0, 0, 1);

  m_visual = new kgmVisual();

  m_body              = new kgmBody();
  m_body->m_udata     = this;
  m_body->m_mass      = 1.0f;
  m_body->m_position  = vec3(0, 0, 1);
  m_body->m_rotation  = vec3(0, 0, 0);
  m_body->m_bound.min = vec3(-1, -1, 0);
  m_body->m_bound.max = vec3(1, 1, 2);

  m_transform.identity();
  m_dvisual.identity();
  m_dvisual.rotate(-0.5f * PI, tv);

  m_health = 1.0f;

  m_animation = null;
  m_skeleton  = null;

  m_state      = null;
  m_gameplayer = false;
}

kgmActor::~kgmActor()
{
  m_inputs.clear();
}

void kgmActor::exit()
{
}

void kgmActor::init()
{
  m_transform.translate(m_body->m_position);

  if(m_visual && m_visual->m_visuals.size() > 0)
  {

  }
}

void kgmActor::update(u32 time)
{
  kgmGameObject::update(time);

  if(m_state && m_state->timeout != 0xffffffff)
  {
    u32 tick = kgmTime::getTicks();
    u32 dtick = tick - m_state->stime;

    if(m_state->timeout < dtick)
    {
      if(m_state->switchto.length() < 1)
        setState("idle", true);
      else
        setState(m_state->switchto, true);
    }
  }

  for(int i = 0; i < m_dummies.size(); i++)
  {
    kgmDummy* dummy = m_dummies[i];

    if(dummy->m_linked)
    {
      kgmGameObject* go = (kgmGameObject*)dummy->m_linked;

      if(go->getBody())
      {
        go->getBody()->translate();
      }
    }
  }
}

void kgmActor::input(u32 btn, int state)
{
  for(int i = 0; i < m_inputs.length(); i++)
  {
    Input in = m_inputs[i];

    if(btn == in.input && state == (int)in.status)
    {
      setState(in.state);
    }
  }
}

void kgmActor::logic(kgmString s)
{
}

void kgmActor::action(kgmString s)
{
}

void kgmActor::setDirection(vec3& d){
  //m_body->m_direction = d;
  //m_body->m_direction.normalize();
  //m_body->m_rotation.z = (float)acos(m_body->m_direction.x);
}

bool kgmActor::setState(kgmString s, bool force)
{
  State* state = null;

  for(kgmList<State*>::iterator i = m_states.begin(); i != m_states.end(); ++i)
  {
    if((*i)->id == s)
    {
      state = (*i);

      break;
    }
  }

  if(!state)
    return false;

  if(!m_state || force || (state->priopity >= m_state->priopity))
  {
    m_state = state;

    if(state->sound && state->sound->m_sound)
    {
      state->sound->m_sound->play((state->timeout == -1) ? (true) : (false));
    }

    m_visual->setAnimation(m_visual->m_animation, state->fstart, state->fend, (state->timeout == -1)?(true):(false));
    state->stime = kgmTime::getTicks();
    action(state->id);
  }
  else
  {
    return false;
  }

  return true;
}
