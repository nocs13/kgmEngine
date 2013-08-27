#pragma once
#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmArchive.h"
#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmMemory.h"
#include "../kgmMedia/kgmSound.h"
#include "../kgmGraphics/kgmAnimation.h"
#include "../kgmGraphics/kgmPicture.h"
#include "../kgmGraphics/kgmTexture.h"
#include "../kgmGraphics/kgmShader.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmPhysics/kgmShapeCollision.h"

#include "kgmGameTools.h"

class kgmIGame;

class kgmGameResources: public kgmIResources{
protected:

 struct Path{

  u32       type;    
  kgmString  path;
  kgmFile    file;
  kgmArchive archive;

  Path(){
   type = 0;
  }

  ~Path(){
   path.clear();
   file.close();
   archive.close();
  }
 };

 kgmIGame*              m_game;

 kgmList<kgmResource*>  m_resources;
 kgmList<Path*>         m_paths;
 kgmGameTools           m_tools;


public:
 kgmGameResources();
 ~kgmGameResources();

 void               release();
 void               add(kgmResource*);
 void               remove(kgmResource*);

 void               addPath(kgmString s);
 bool               getFile(char* id, kgmMemory<char>& m);

 kgmPicture*        getPicture(char* id);
 kgmTexture*        getTexture(char* id);
 kgmMaterial*       getMaterial(char* id);
 kgmShader*         getShader(char* id);
 kgmAnimation*      getAnimation(char* id);
 kgmSound*          getSound(char*);
 kgmMesh*           getMesh(char* id);
 kgmSkeleton*       getSkeleton(char* id);
 kgmFont*           getFont(char* id, u32 r, u32 c);
 kgmShapeCollision* getShapeCollision(char* id);
};
