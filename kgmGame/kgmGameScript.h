#pragma once
#include "../kgmMain/kgmObject.h"
#include "../kgmScript/kgmLua.h"

class kgmGameScript: public kgmObject{
 public:
  enum Type{
    Lua,
    Java,
  };

  kgmLua m_lua;
 public:
  kgmGameScript();
  ~kgmGameScript();

  bool add(kgmGameScript::Type type, kgmString script);
  bool run(kgmGameScript::Type type, kgmString script);

  //events
  virtual void onLoad();
  virtual void onQuit();
  virtual void onIdle();
};
