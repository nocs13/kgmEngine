#include "kgmGameScript.h"
#include "kgmIGame.h"

kgmGameScript::kgmGameScript()
{

}

kgmGameScript::~kgmGameScript()
{

}

bool kgmGameScript::add(kgmGameScript::Type type, kgmString script)
{
  m_lua.load(script);
  return true;
}

bool kgmGameScript::run(kgmGameScript::Type type, kgmString script)
{
  m_lua.run(script);
  return true;
}

void kgmGameScript::onLoad()
{
  m_lua.call("onLoad","");
}

void kgmGameScript::onQuit()
{
  m_lua.call("onQuit","");
}

void kgmGameScript::onIdle()
{
  m_lua.call("onIdle","");
}

