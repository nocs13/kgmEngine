#include "kgmBaseActions.h"

kgmVariable kgmBaseActions::find(kgmList<kgmVariable>& variables, kgmString id)
{
  kgmVariable v;

  for (kgmList<kgmVariable>::iterator i = variables.begin(); i != variables.end(); ++i)
  {
    if ((*i).getName() == id)
      return (*i);
  }

  return v;
}

bool kgmBaseActions::get(kgmList<kgmVariable>& variables, kgmString id, int& value)
{
  kgmVariable v = find(variables, id);

  if (v.getType() != kgmVariable::TInteger)
    return false;

  value = v.getInteger();

  return true;
}

bool kgmBaseActions::get(kgmList<kgmVariable>& variables, kgmString id, float& value)
{
  kgmVariable v = find(variables, id);

  if (v.getType() != kgmVariable::TFloat)
    return false;

  value = v.getFloat();

  return true;
}

bool kgmBaseActions::get(kgmList<kgmVariable>& variables, kgmString id, kgmString& value)
{
  kgmVariable v = find(variables, id);

  if (v.getType() != kgmVariable::TString)
    return false;

  value = v.getString();

  return true;
}

void kgmBaseActions::register_actions()
{
  kgmActor::g_actions.set("nothing", nothing);
  kgmActor::g_actions.set("accelerate", accelerate);
}

void kgmBaseActions::nothing(kgmIGame*, kgmActor*, kgmActor::Action*)
{

}

void kgmBaseActions::accelerate(kgmIGame* g, kgmActor* a, kgmActor::Action* c)
{
  u32 ms = kgmTime::getTicks() - c->time;
  float acc = 0.0;

  if(!a->body())
    return;

  get(c->variables, "accelerate", acc);

  a->body()->m_velocity += acc * ms / 1000;
}
