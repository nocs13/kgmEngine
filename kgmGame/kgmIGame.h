#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmPointer.h"

#include "../kgmMath/kgmMath.h"

enum game_condition
{
  gcnd_none,
  gcnd_pawnkilled,
  gcnd_pawnproximity,
  gcnd_anyproximity,
};

enum game_action
{
  gact_none,
  gact_gamepause,
  gact_gameresume,
  gact_gameoverfail,
  gact_gameoversuccess,
  gact_stageoverfail,
  gact_stageoversuccess,
  gact_nextstage,
  gact_nextlevel,
  gact_collision,


  gact_open,
  gact_close,
  gact_lock,
  gact_unlock,
  gact_enable,
  gact_disable,
  gact_show,
  gact_hide,
  gact_died,
  gact_interogate,
};

enum game_action_type
{
  gact_type_none,
  gact_type_input,
  gact_type_object,
};

enum game_state
{
  gstate_idle,
  gstate_run,
  gstate_load,
  gstate_clean,
  gstate_pause,
};

enum game_button
{
  gbtn_none = 0,

  gbtn_up,
  gbtn_down,
  gbtn_left,
  gbtn_right,

  gbtn_exit,
  gbtn_pause,
  gbtn_start,

  gbtn_a,
  gbtn_b,
  gbtn_c,
  gbtn_d,
  gbtn_e,
  gbtn_f,
  gbtn_g,
  gbtn_h,
  gbtn_i,
  gbtn_j,
  gbtn_k,
  gbtn_l,
  gbtn_m,
  gbtn_n,
  gbtn_o,
  gbtn_p,
  gbtn_q,
  gbtn_r,
  gbtn_s,
  gbtn_t,
  gbtn_u,
  gbtn_v,
  gbtn_w,
  gbtn_x,
  gbtn_y,
  gbtn_z,

  grot_x,
  grot_y,
  grot_z,

  gbtn_end = 64
};


enum game_event
{
  gevt_none,
};

struct kgmGameCommand
{
  char* m_cmd;
};

class kgmGui;
class kgmBody;
class kgmUnit;
class kgmActor;
class kgmLight;
class kgmVisual;
class kgmIGC;
class kgmIAudio;
class kgmIVideo;
class kgmILogic;
class kgmISpawner;
class kgmIPhysics;
class kgmIGraphics;
class kgmIResources;

class kgmSystem;
class kgmWindow;
class kgmEnvironment;

class kgmGameNode;

class kgmIGame
{
public:
  enum State
  {
    State_Idle,
    State_Quit,
    State_Play,
    State_Load,
    State_Clean,
    State_Pause,
    State_Edit
  };

  struct Editor
  {

  };

public:
  //virtuals
  virtual int            gLoad(kgmString)     = 0;    //load game map
  virtual int            gUnload()            = 0;    //unload game map
  virtual int            gCommand(kgmString)  = 0;    //do command
  virtual int            gQuit()              = 0;    //close game
  virtual int            gButton(game_button) = 0;    //game input button state
  virtual u32            gState()             = 0;    //check game active  state
  virtual int            gSwitch(u32)         = 0;    //switch game state
  virtual kgmActor*      gSpawn(kgmString)    = 0;    //spawn game actor
  virtual kgmUnit*       gObject(kgmString)   = 0;    //spawn game object

  virtual bool           gAppend(kgmGameNode*) = 0;            //add game node in map
  virtual u32            gObjects(kgmList<kgmGameNode*>&) = 0; //collect nodes in map

  virtual void           guiAdd(kgmGui* g) = 0;

  virtual kgmIPhysics*    getPhysics() = 0;
  virtual kgmISpawner*    getSpawner() = 0;
  virtual kgmIAudio*      getAudio() = 0;
  virtual kgmIVideo*      getVideo() = 0;
  virtual kgmILogic*      getLogic() = 0;
  virtual kgmIGraphics*   getGraphics() = 0;
  virtual kgmIResources*  getResources() = 0;
  virtual kgmWindow*      getWindow() = 0;
  virtual kgmSystem*      getSystem() = 0;
  virtual kgmEnvironment* getEnvironment() = 0;

protected:
  virtual void  initResources() = 0;
  virtual void  initGraphycs() = 0;
  virtual void  initPhysics() = 0;
  virtual void  initSystem() = 0;
  virtual void  initAudio() = 0;
  virtual void  initLogic() = 0;

public:
  static kgmIGame* getGame();
};

