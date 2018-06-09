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
 public:
  kgmLuaScript(kgmIResources *);
  ~kgmLuaScript();

  bool load(kgmString id);
  bool set(kgmString name, void* fn);
  void* call(kgmString name, kgmString fmt, ...);
  void push(int arg);
  void push(double arg);
  void push(char* arg);
  void push(void* arg);
};
