#pragma once

#include "kgmIGame.h"
#include "kgmGameResources.h"
#include "kgmGameGraphics.h"
#include "kgmGameThreader.h"
#include "kgmGamePhysics.h"
#include "kgmGameLogic.h"
#include "kgmGameAudio.h"

#include "kgmUnit.h"
#include "kgmActor.h"

#include "kgmGameSettings.h"

#include "../kgmBase/kgmQuadTree.h"
#include "../kgmBase/kgmHArray.h"
#include "../kgmSystem/kgmThread.h"
#include "../kgmSystem/kgmWindow.h"
#include "../kgmSystem/kgmOGLWindow.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGraphics.h"

#ifdef EDITOR
#include "editor/kEditor.h"

using namespace kgmGameEditor;
#endif

class kgmGameBase: public kgmOGLWindow, public kgmIGame
{
public:
  static kgmGameBase* m_game;

protected:
  kgmIVideo*        m_video;
  kgmGameAudio*     m_audio;
  kgmGamePhysics*   m_physics;
  kgmGameResources* m_resources;
  kgmGameSettings*  m_settings;

  kgmSystem* m_system;

  u32 m_width;
  u32 m_height;
  u32 m_bpp;
  u32 m_fullscreen;

  kgmFont*  m_font;
  kgmString m_paths;

  char m_keys[130];
  char m_input[gbtn_end];
  char m_keymap[150];

  int  m_state;
  bool m_result;

  kgmGameLogic*    m_logic;
  kgmGameGraphics* m_render;

  kgmList<kgmGui*>      m_guis;
  kgmList<kgmGameNode*> m_nodes;
  kgmHArray<kgmString, kgmGameNode*> m_gnodes;

  u32             m_fps;
  kgmGameThreader m_threader_1;

#ifdef EDITOR
  friend class kEditor;

  kEditor* editor = null;

public:
  kEditor* getEditor()
  {
    return editor;
  }

private:
#endif

public:
  kgmGameBase(bool edit = false);
  kgmGameBase(kgmString &conf);
  virtual ~kgmGameBase();

  kgmIPhysics*       getPhysics();
  kgmISpawner*       getSpawner();
  kgmIAudio*         getAudio();
  kgmIVideo*         getVideo();
  kgmILogic*         getLogic();
  kgmIGraphics*      getGraphics();
  kgmIResources*     getResources();
  kgmSystem*         getSystem();
  kgmWindow*         getWindow();
  kgmEnvironment*    getEnvironment();

  void quit();
  void log(const char* msg);

  void onEvent(kgmEvent::Event*);
  void onIdle();
  void onClose();
  void onKeyUp(int k);
  void onKeyDown(int k);
  void onMsMove(int k, int x, int y);
  void onMsWheel(int k, int x, int y, int z);
  void onMsLeftUp(int k, int x, int y);
  void onMsLeftDown(int k, int x, int y);
  void onMsRightUp(int k, int x, int y);
  void onMsRightDown(int k, int x, int y);
  void onResize(int w, int h);
  void onPaint(kgmIGC*);
  void onAction(kgmGui* e, u32 arg);

  void onInputAction(int action, int argument);

  int            gLoad(kgmString);        //load game level
  int            gUnload();               //unload game level
  int            gCommand(kgmString);     //do string command
  int            gQuit();                 //quit from level
  int            gButton(game_button);
  u32            gState();                //check game state
  int            gSwitch(u32);            //switch game state
  void           gPause(bool);            //render game scene
  void           gRender();               //render game scene
  kgmActor*      gSpawn(kgmString);       //spawn the actor
  kgmUnit*       gObject(kgmString);      //spawn game object
  bool           gAppend(kgmGameNode*);   //add game node in map
  u32            gObjects(kgmList<kgmGameNode*>&);
  bool           gAppend(kgmBody*);       //add game body in map
  bool           gAppend(kgmLight*);      //add game light in map
  bool           gAppend(kgmVisual*);     //add game object in map

  bool gMapBinary(kgmString&);
  bool gMapAscii(kgmString&);
  bool gMapXml(kgmString&);

  //init interfaces
  void  initResources();
  void  initGraphycs();
  void  initPhysics();
  void  initSystem();
  void  initAudio();
  void  initLogic();

  void guiAdd(kgmGui* g)
  {
    if(g)
    {
      for(int i = 0; i < m_guis.size(); i++)
      {
        if(g == m_guis[i])
        {
          return;
        }
      }

      m_guis.add(g);

      if(m_render)
        m_render->add(g);
    }
  }

  kgmGraphics* getRender()
  {
    return m_render;
  }

  kgmGameSettings* getSettings()
  {
    return m_settings;
  }

  s32 getKeyState(u8 key)
  {
    return m_keys[key];
  }

private:
  bool  loadXml(kgmString& path);

  static int doLogic(kgmGameBase*);
  static int doPhysics(kgmGameBase*);
};
