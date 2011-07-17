#pragma once

#include "kgmIGame.h"
#include "kgmGameResources.h"
#include "kgmGamePhysics.h"
#include "kgmGameLogic.h"
#include "kgmGameScript.h"
#include "kgmActor.h"

#include "../kgmMain/kgmQuadTree.h"
#include "../kgmSystem/kgmWindow.h"
#include "../kgmSystem/kgmOGLWindow.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGraphics.h"


//#include "../kgmLighting.inl"
#ifdef WIN32
//#include <fstream>
#endif
#ifdef LINUX
//#include <fstream>
#endif


class kgmGameBase: public kgmIGame, public kgmOGLWindow
{
 public:
 class Scene{
 public:
   kgmNode*            node;
   kgmCamera*          camera;
   kgmList<kgmObject*> objects;
   kgmList<kgmActor*>  actors;

   Scene(){
    node = new kgmNode;
    camera = 0;
   }

   ~Scene(){
    for(int i = 0; i < objects.size(); i++)
      objects[i]->release();
    for(int i = 0; i < actors.size(); i++)
      actors[i]->release();
    if(node)
      delete node;
    objects.clear();
    actors.clear();
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

 kgmSystem*         m_system;
 kgmGraphics*       m_render;

 uint		    m_width;
 uint		    m_height;
 uint		    m_bpp;
 uint		    m_fullscreen;

 kgmFont*       m_font;
 kgmString      m_paths;

 char           m_input[gbtn_end];
 char           m_keymap[150];


public:
 kgmGameLogic*      m_logic;    //game logic manager
 kgmGameScript      m_script;   //game scripts
 int		    m_state;	//game state
 bool		    m_result;	//game over result

 //Display data
 kgmList<kgmGui*>   m_guis;     //game or nongame guis
 kgmList<Scene*>    m_scenes;   //game nodes

 ////////////////
public:
 kgmGameBase();
 virtual ~kgmGameBase();

 kgmIGraphics*		getGraphics();
 kgmIPhysics*		getPhysics();
 kgmISpawner*		getSpawner();
 kgmIAudio*		getAudio();
 kgmIVideo*		getVideo();
 kgmIResources*		getResources();
 kgmSystem*		getSystem();

 void quit();
 void log(const char* msg);
 void settings(bool save = false);

 void onEvent(kgmEvent::Event*);
 void onIdle();
 void onKeyUp(int k);
 void onKeyDown(int k);
 void onMsMove(int k, int x, int y);
 void onResize(int w, int h);
 void onPaint(kgmIGraphics*);
 void onAction(kgmGui* e, uint arg);

 void gStart();		    //start game
 int  gLoad(kgmString);     //load game level
 int  gCommand(kgmString);  //do string command
 int  gQuit();              //quit from level
 uint gState();             //check game state
 bool gResult();	    //check game end status
 void gRender();            //render game scene
 void gLogic();             //do game logic  & Physics
 kgmActor*    gSpawn(kgmString);	//spawns and add the actor

 bool         gMapBinary(kgmString&);
 bool         gMapAscii(kgmString&);
 bool         gMapXml(kgmString&);

///////////////////////////////////////
protected:
// Game Resources
 kgmTexture*    getTexture(char*);
 kgmShader*	getShader(char*);
 kgmMesh*	getMesh(char*);
 kgmActor*	getActor(kgmString);
 kgmAnimation*	getAnimation(char*);

 Scene* loadXml(kgmString& path);

 void add(kgmGui* g){
   if(g){
       m_guis.add(g);
       if(m_render)
         m_render->add(g);
   }
 }
 void add(Scene* s){
   if(s){
       m_scenes.add(s);
       if(m_render)
         m_render->add(s->node);
   }
 }
public:
/////// inlines
};


