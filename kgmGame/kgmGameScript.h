#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmSystem/kgmLua.h"

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
