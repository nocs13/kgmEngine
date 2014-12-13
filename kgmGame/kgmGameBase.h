#pragma once

#include "kgmIGame.h"
#include "kgmGameResources.h"
#include "kgmGameGraphics.h"
#include "kgmGamePhysics.h"
#include "kgmGameLogic.h"
#include "kgmGameAudio.h"

#include "kgmGameObject.h"
#include "kgmActor.h"

#include "kgmGameSettings.h"

#include "../kgmBase/kgmQuadTree.h"
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

  enum NodeType
  {
    NodeNone,
    NodeMesh,
    NodeLight,
    NodeCamera,
    NodeMaterial,
    NodeActor,
    NodeSensor,
    NodeTrigger,
    NodeGameObject
  };

  class Node
  {
    NodeType type;

    kgmObject* object;

    vec3       position;
    vec3       rotation;

    box3       bound;

    kgmString  name;
    kgmString  link;
    kgmString  material;
    kgmString  shader;
    kgmString  shape;

    bool       collision;
    bool       hidden;
    bool       lock;

  public:
    Node()
    {
      object = null;
      type   = NodeNone;
    }
  };

protected:
  kgmIVideo*         m_video;
  kgmGameAudio*      m_audio;
  kgmGamePhysics*    m_physics;
  kgmGameResources*  m_resources;
  kgmGameSettings*   m_settings;

  kgmSystem*         m_system;

  u32		    m_width;
  u32		    m_height;
  u32		    m_bpp;
  u32		    m_fullscreen;

  kgmFont*       m_font;
  kgmString      m_paths;

  char           m_keys[130];
  char           m_input[gbtn_end];
  char           m_keymap[150];

  bool           m_gamemode;

public:
  int            m_state;   //game state
  bool           m_result;  //game over result

  kgmGameLogic*         m_logic;
  kgmGameGraphics*      m_render;
  kgmList<kgmGui*>      m_guis;    //game or nongame guis

#ifdef EDITOR
  friend class kEditor;

  kEditor* editor;
  bool     editing;

  void     setEditing(bool e) { editing = e; }
  kEditor* getEditor(){ return editor; }
#endif

public:
  kgmGameBase();
  kgmGameBase(kgmString &conf);
  virtual ~kgmGameBase();

  kgmIGC*            getGC();
  kgmIPhysics*       getPhysics();
  kgmISpawner*       getSpawner();
  kgmIAudio*         getAudio();
  kgmIVideo*         getVideo();
  kgmILogic*         getLogic();
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
  u32            gState();                //check game state
  int            gSwitch(u32);            //switch game state
  void           gPause(bool);            //render game scene
  void           gRender();               //render game scene
  kgmActor*      gSpawn(kgmString);       //spawns the actor
  bool           gAppend(kgmGameObject*); //add game object in scene
  kgmGameObject* gObject(kgmString);     //spawn game object

  bool gMapBinary(kgmString&);
  bool gMapAscii(kgmString&);
  bool gMapXml(kgmString&);

  ///////////////////////////////////////

public:
  //init interfaces
  void  initResources();
  void  initGraphycs();
  void  initPhysics();
  void  initSystem();
  void  initAudio();
  void  initLogic();

  // Game Resources
  kgmTexture*    getTexture(char*);
  kgmShader*     getShader(char*);
  kgmMesh*       getMesh(char*);
  kgmActor*      getActor(kgmString);
  kgmAnimation*  getAnimation(char*);

  bool           loadXml(kgmString& path);


public:
  void guiAdd(kgmGui* g)
  {
    if(g)
    {
      bool is = false;

      for(int i = 0; i < m_guis.size(); i++)
      {
        if(g == m_guis[i])
        {
          is = true;

          break;
        }
      }

      if(!is)
      {
        m_guis.add(g);
        g->increment();

        if(m_render)
          m_render->add(g);
      }
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
};
