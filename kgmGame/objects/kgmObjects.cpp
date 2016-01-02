#include "kgmObjects.h"
KGMOBJECT_IMPLEMENT(kgmResult, kgmUnit);
KGMOBJECT_IMPLEMENT(kgmParticlesObject, kgmEffect);
KGMOBJECT_IMPLEMENT(kgmFlame, kgmParticlesObject);
KGMOBJECT_IMPLEMENT(kgmSmoke, kgmParticlesObject);
KGMOBJECT_IMPLEMENT(kgmExplode, kgmParticlesObject);
KGMOBJECT_IMPLEMENT(kgmLaser, kgmEffect);
KGMOBJECT_IMPLEMENT(kgmIonNozzle, kgmEffect);

kgmResult::kgmResult(kgmIGame* g, u32 time, s32 res, kgmString txt)
  :kgmUnit(g)
{
  text = txt;
  result = res;
  timeout(time);

  int cx, cy, cw, ch, tw, tl;
  //game->getWindow()->getRect(cx, cy, cw, ch);

  tl = txt.length();

  if(tl < 1)
    tl = 1;

  tw = 300 / tl;

  m_visual  = new kgmVisual();
  m_text = new kgmText();
  m_text->m_rect  = uRect(10, ch - 2 * tw, 300, 2 * tw);
  m_text->m_text  = txt;
  m_text->m_size  = tw;

  m_visual->set(m_text);
}

kgmResult::~kgmResult()
{
  delete m_text;
}

s32 kgmResult::getResult()
{
  return result;
}

void kgmResult::update(u32 t)
{
  kgmUnit::update(t);
}

void kgmResult::init()
{
}

void kgmResult::exit()
{
}

