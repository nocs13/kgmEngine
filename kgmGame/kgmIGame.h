#pragma once
#include "../kgmMain/kgmInterface.h"
#include "../kgmMain/kgmTypes.h"
#include "../kgmMain/kgmString.h"

enum game_condition{
 gcnd_none,
 gcnd_pawnkilled,
 gcnd_pawnproximity,
 gcnd_anyproximity,
};

enum game_action{
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

enum game_state{
 gstate_none,
 gstate_load,
 gstate_run,
 gstate_finish,
 gstate_pause,
};

enum game_button{
 gbtn_none = 0,

 gbtn_up,
 gbtn_down,
 gbtn_left,
 gbtn_right,

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

 grot_x,
 grot_y,
 grot_z,
 
 gbtn_end = 64,
};


enum game_event{
 gevt_none,
};

struct kgmGameCommand{
 char* m_cmd;
};

class kgmActor;
class kgmIAudio;
class kgmIVideo;
class kgmISpawner;
class kgmIPhysics;
class kgmIGraphics;
class kgmIResources;

class kgmSystem;
class kgmEnvironment;

class kgmIGame: public kgmInterface{
public:
 struct Event{
  uint	    evt_id;
  kgmActor* evt_from;
  kgmActor* evt_to;
 };
 struct Input{
  uint action;
  int  state;
  int  value;
 };

public:
 enum State{
  State_None,
  State_Play,
  State_Quit,
  State_Load,
  State_Pause,
  State_Stop,
 };


public:  //virtuals
 virtual int           gLoad(kgmString) = 0;  //load game map
 virtual int           gCommand(kgmString) = 0;    //do command
 virtual int           gQuit() = 0;       //close game
 virtual uint	       gState() = 0;      //check game active  state
 virtual bool	       gResult() = 0;     //chec result game
 virtual kgmActor*     gSpawn(kgmString) = 0;

 //usfull interfaces
 virtual kgmIGraphics*		getGraphics() = 0;
 virtual kgmIPhysics*		getPhysics() = 0;
 virtual kgmISpawner*		getSpawner() = 0;
 virtual kgmIAudio*		getAudio() = 0;
 virtual kgmIVideo*		getVideo() = 0;
 virtual kgmIResources*		getResources() = 0;
 virtual kgmSystem*		getSystem() = 0;

public:		//statics for help
 static kgmIGame*		getGame();
};

