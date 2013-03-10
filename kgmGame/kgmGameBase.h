#pragma once

#include "kgmIGame.h"
#include "kgmGameResources.h"
#include "kgmGameGraphics.h"
#include "kgmGamePhysics.h"
#include "kgmActor.h"
#include "kgmGameNode.h"

#include "kgmGameSettings.h"

#include "../kgmBase/kgmQuadTree.h"
#include "../kgmSystem/kgmWindow.h"
#include "../kgmSystem/kgmOGLWindow.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGraphics.h"

#ifdef WIN32
#endif
#ifdef LINUX
#endif

class kgmGameBase: public kgmIGame, public kgmOGLWindow
{
 
public:
  class Scene{
    kgmList<kgmObject*> objects;

  public:
    Scene(){
    }
    ~Scene(){
      for(int i = 0; i < objects.size(); i++){
        objects[i]->release();
      }
      objects.clear();
    }
  };

public:
 static kgmGameBase*	m_game;

protected:
 kgmIAudio*		m_audio;
 kgmIVideo*		m_video;
 kgmISpawner*		m_spawner;
 kgmIGraphics*		m_graphics;
 kgmGamePhysics*	m_physics;
 kgmGameResources*	m_resources;
 kgmGameSettings* m_settings;

 kgmSystem*         m_system;
 //kgmGraphics*       m_render;

 u32		    m_width;
 u32		    m_height;
 u32		    m_bpp;
 u32		    m_fullscreen;

 kgmFont*       m_font;
 kgmString      m_paths;

 char           m_input[gbtn_end];
 char           m_keymap[150];

 kgmList<Idle*> m_idles;
 kgmList<GameSensor*> m_sensors;
 kgmList<InputEventListener*> m_eventListeners;
public:
 int		    m_state;	//game state
 bool		   m_result;	//game over result

 kgmGameGraphics*      m_render;
 kgmList<kgmObject*>   m_world;

 //kgmList<Scene*>    m_scenes;

private:
 //Display data
 kgmList<kgmGui*>   m_guis;     //game or nongame guis
public:
 kgmGameNode*    m_node;   //game nodes

 ////////////////
public:
 kgmGameBase();
 kgmGameBase(kgmString &conf);
 virtual ~kgmGameBase();

 kgmIGraphics*		 getGraphics();
 kgmIPhysics*		  getPhysics();
 kgmISpawner*		  getSpawner();
 kgmIAudio*	     getAudio();
 kgmIVideo*		    getVideo();
 kgmIResources*		getResources();
 kgmSystem*		    getSystem();

 void quit();
 void log(const char* msg);

 void onEvent(kgmEvent::Event*);
 void onIdle();
 void onKeyUp(int k);
 void onKeyDown(int k);
 void onMsMove(int k, int x, int y);
 void onResize(int w, int h);
 void onPaint(kgmIGraphics*);
 void onAction(kgmGui* e, u32 arg);

 void onInputAction(int action, int argument);

 void gStart();		    //start game
 int  gLoad(kgmString);     //load game level
 int  gUnload();     //load game level
 int  gCommand(kgmString);  //do string command
 int  gQuit();              //quit from level
 u32  gState();             //check game state
 void gRender();            //render game scene
 void gLogic();             //do game logic  & Physics

 bool gMapBinary(kgmString&);
 bool gMapAscii(kgmString&);
 bool gMapXml(kgmString&);

 kgmActor*    gSpawn(kgmString);	//spawns and add the actor
 kgmGameNode* gMainNode(){ return m_node; }

///////////////////////////////////////
protected:
// Game Resources
 kgmTexture*    getTexture(char*);
 kgmShader*	getShader(char*);
 kgmMesh*	getMesh(char*);
 kgmActor*	getActor(kgmString);
 kgmAnimation*	getAnimation(char*);

 kgmGameNode* loadXml(kgmString& path);
 bool loadXml_II(kgmString& path);


public:
 void addGui(kgmGui* g)
 {
   if(g){
     bool is = false;
     for(int i = 0; i < m_guis.size(); i++)
       if(g == m_guis[i]) { is = true; break; }
     if(!is){
       m_guis.add(g);
       if(m_render)
         m_render->add(g);
     }
   }
 }
 
 void addGameSensor(GameSensor* g){
   if(g)
     m_sensors.add(g);
 }

 void addGameIdle(Idle* i){
   if(i)
     m_idles.add(i);
 }

 bool addInputEventListener(InputEventListener* gel){
   if(gel)
     m_eventListeners.add(gel);

   return true;
 }

 kgmGraphics* getRender(){
     return 0; //m_render;
 }
public:
/////// inlines
};


