#pragma once

#include "kgmIGame.h"
#include "kgmGameAI.h"
#include "kgmGameResources.h"
#include "kgmGameGraphics.h"
#include "kgmGamePhysics.h"
#include "kgmGameScript.h"
#include "kgmGameInput.h"
#include "kgmGameAudio.h"
#include "kgmGameMessenger.h"
#include "kgmGameRetention.h"

#include "kgmUnit.h"

#include "kgmGameSettings.h"

#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmQuadTree.h"
#include "../kgmSystem/kgmThread.h"
#include "../kgmSystem/kgmWindow.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGraphics.h"

class kgmGameMap;

class kgmGameBase: public kgmWindow, public kgmIGame
{
  KGM_OBJECT(kgmGameBase)
  friend class kgmGameMap;

public:
  static kgmGameBase* m_game;
  static bool         m_need_editor;

protected:
  kgmIGC*           m_gc;
  kgmIVideo*        m_video;
  kgmGameAI*        m_ai;
  kgmGameAudio*     m_audio;
  kgmGameInput*     m_input;
  kgmGameScript*    m_script;
  kgmGamePhysics*   m_physics;
  kgmGameGraphics*  m_graphics;
  kgmGameSettings*  m_settings;
  kgmGameResources* m_resources;
  kgmGameMessenger* m_messenger;
  kgmGameRetention* m_retention;

  kgmSystem* m_system;

  u32 m_width;
  u32 m_height;
  u32 m_bpp;
  u32 m_fullscreen;

  kgmFont*  m_font;
  kgmString m_paths;

  u8 m_keys[128];
  u8 m_keymap[128];

  s32 m_point[3];

  s32  m_state;
  bool m_result;

  kgmList<kgmGui*>    m_guis;
  kgmList<kgmUnit*>   m_units;
  kgmList<kgmObject*> m_objects;

  kgmMap<kgmString, NEW_UNIT> m_unit_generators;

  u32              m_fps;
  u32              m_tpf;

private:
  struct UnitIterator: public kgmIGame::Iterator
  {
    kgmGameBase* game;

    kgmList<kgmUnit*>::iterator i;

  public:
    UnitIterator(kgmGameBase* g)
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
  kgmGameBase();
  ~kgmGameBase();

  kgmIGC*            getGC();
  kgmIAI*            getAI();
  kgmIPhysics*       getPhysics();
  kgmISpawner*       getSpawner();
  kgmIAudio*         getAudio();
  kgmIVideo*         getVideo();
  kgmIInput*         getInput();
  kgmIScript*        getScript();
  kgmIGraphics*      getGraphics();
  kgmIResources*     getResources();
  kgmSystem*         getSystem();
  kgmWindow*         getWindow();

  Messenger*         getMessenger();

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

  int            gQuit();
  int            gInit();
  int            gLoad(kgmString);
  int            gUnload();
  int            gButton(game_button);
  u32            gState();
  int            gSwitch(u32);
  void           gPause(bool);
  void           gRender();
  kgmUnit*       gSpawn(kgmString);
  bool           gAppend(kgmUnit*);
  kgmUnit*       gUnit(kgmString);
  Iterator*      gUnits();
  bool           gUnitRegister(kgmString, NEW_UNIT);

  bool gMapBinary(kgmString&);
  bool gMapAscii(kgmString&);
  bool gMapXml(kgmString&);

  //init interfaces
  void  initResources();
  void  initSettings();
  void  initGraphycs();
  void  initPhysics();
  void  initSystem();
  void  initAudio();
  void  initInput();
  bool  initScript();
  void  initGC();
  void  initAI();

  void guiAdd(kgmGui* g);

  kgmGraphics* getRender()
  {
    return m_graphics;
  }

  kgmGameSettings* getSettings()
  {
    return m_settings;
  }

  kgmGameRetention* getRetention()
  {
    return m_retention;
  }

  s32 getKeyState(u8 key)
  {
    return m_keys[key];
  }

  static void needEditor(bool s)
  {
    kgmGameBase::m_need_editor = s;
  }

private:
  bool  loadXml(kgmString& path);
};
