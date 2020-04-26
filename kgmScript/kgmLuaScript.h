#include "kgmIScript.h"
#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmString.h"

class lua_State;
class kgmIResources;

class kgmLuaScript: public kgmIScript
{
  lua_State* handler = null;

  kgmIResources* resources = null;

  void* r_call = null;

  void* X = null;

  s32 m_carg = 0;

 public:
  kgmLuaScript(kgmIResources *);
  ~kgmLuaScript();

  bool load(kgmString id);
  bool set(kgmString name,  FN);
  bool args(kgmString fmt, ...);
  bool resl(kgmString fmt, ...);
  void* call(kgmString name, kgmString fmt, ...);

  void  setX(void* x);
  void* getX();

private:
  void push(int arg);
  void push(double arg);
  void push(char* arg);
  void push(void* arg);
  void pop(int* arg);
  void pop(double* arg);
  void pop(char** arg);
  void pop(void** arg);
};
