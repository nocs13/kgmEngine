#include "kgmActor.h"
KGMOBJECT_IMPLEMENT(kgmActor,	kgmGameObject);

kgmActor::kgmActor()
{
 vec3 tv(0, 0, 1);

 m_visual = new kgmVisual();

 m_body              = new kgmBody();
 m_body->m_udata     = this;
 m_body->m_mass      = 1.0f;
 m_body->m_position  = vec3(0, 0, 1);
 m_body->m_rotation  = vec3(0, 0, 0);
 m_body->m_direction = vec3(1, 0, 0);
 m_body->m_bound.min = vec3(-1, -1, 0);
 m_body->m_bound.max = vec3(1, 1, 2);

 m_transform.identity();
 m_dvisual.identity();
 m_dvisual.rotate(-0.5f * PI, tv);

 m_birth  = kgmTime::getTicks();
 m_health = 1.0f;

 m_animation = null;
 m_skeleton  = null;

 m_state      = null;
 m_gameplayer = false;
}

kgmActor::~kgmActor(){
  for(int i = 0; i < m_childs.length(); i++)
    m_childs[i]->release();

  m_childs.clear();

  delete m_body;
  m_visual->release();
}

void kgmActor::exit()
{
}

void kgmActor::init()
{
 m_transform.translate(m_body->m_position);
 //m_body->m_gravity = true;

 if(m_visual && m_visual->m_visuals.size() > 0)
 {

 }
}

void kgmActor::update(u32 time)
{
    mtx4 tm;

    m_body->transform(tm);
    m_visual->m_transform = tm;
}

void kgmActor::input(u32 btn, int state)
{

}

void kgmActor::setDirection(vec3& d){
 m_body->m_direction = d;
 m_body->m_direction.normalize();
 m_body->m_rotation.z = (float)acos(m_body->m_direction.x);
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

        if(state->sound != (kgmSound*)-1)
        {
            if(state->sound && state->sound->m_sound)
                state->sound->m_sound->play((state->time == -1)?(true):(false));
        }

        m_visual->setAnimation(m_visual->m_animation, state->fstart, state->fend, (state->time == -1)?(true):(false));
    }
    else
    {
        return false;
    }

    return true;
}
