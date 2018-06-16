#pragma once

#include "kgmIGame.h"
#include "kgmGameResources.h"
#include "kgmGameGraphics.h"
#include "kgmGameThreader.h"
#include "kgmGamePhysics.h"
#include "kgmGameScript.h"
#include "kgmGameLogic.h"
#include "kgmGameAudio.h"

#include "kgmUnit.h"
#include "kgmActor.h"

#include "kgmGameSettings.h"

#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmQuadTree.h"
#include "../kgmSystem/kgmThread.h"
#include "../kgmSystem/kgmWindow.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGraphics.h"

#ifdef EDITOR
#include "editor/kEditor.h"

using namespace kgmGameEditor;
#endif

class kgmGameMap;

class kgmGameBase: public kgmWindow, public kgmIGame
{
  friend class kgmGameMap;
public:
  static kgmGameBase* m_game;

protected:
  kgmIGC*           m_gc;
  kgmIVideo*        m_video;
  kgmGameAudio*     m_audio;
  kgmGameScript*    m_script;
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
  kgmGameGraphics* m_graphics;

  kgmList<kgmGui*>    m_guis;
  kgmList<kgmUnit*>   m_units;
  kgmList<kgmObject*> m_objects;

  u32             m_fps;
  u32             m_tpf;
  kgmGameThreader m_threader;

#ifdef EDITOR
  friend class kEditor;

  kEditor* editor = null;

public:
  kEditor* getEditor()
  {
    return editor;
  }
#endif

private:
  struct GIterator: public kgmIGame::Iterator
  {
    kgmGameBase* game;

    kgmList<kgmUnit*>::iterator i;

  public:
    GIterator(kgmGameBase* g)
    {
      game = g;
      i = g->m_units.begin();
    }

    kgmUnit* next()
    {
      if (i.end())
        return null;

      kgmUnit* u = (*i);

      ++i;

      return u;
    }
  };

public:
  kgmGameBase(bool edit = false);
  kgmGameBase(kgmString &conf);
  virtual ~kgmGameBase();

  kgmIGC*            getGC();
  kgmIPhysics*       getPhysics();
  kgmISpawner*       getSpawner();
  kgmIAudio*         getAudio();
  kgmIVideo*         getVideo();
  kgmILogic*         getLogic();
  kgmIScript*        getScript();
  kgmIGraphics*      getGraphics();
  kgmIResources*     getResources();
  kgmSystem*         getSystem();
  kgmWindow*         getWindow();

  u32                timeUpdate();

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
  void onPaint();

  void onInputAction(int action, int argument);

  int            gLoad(kgmString);        //load game level
  int            gUnload();               //unload game level
  int            gQuit();                 //quit from level
  int            gButton(game_button);
  u32            gState();                //check game state
  int            gSwitch(u32);            //switch game state
  void           gPause(bool);            //render game scene
  void           gRender();               //render game scene
  kgmUnit*       gSpawn(kgmString);       //spawn the actor
  bool           gAppend(kgmUnit*);       //add game node in map
  kgmUnit*       gUnit(kgmString);        // get unit by name
  Iterator*      gObjects();

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
  void  initScript();
  void  initGC();

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

      if(m_graphics)
        m_graphics->add(g);
    }
  }

  kgmGraphics* getRender()
  {
    return m_graphics;
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
  static int doScript(kgmGameBase*);
};
