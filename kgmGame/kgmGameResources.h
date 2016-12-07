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
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmPhysics/kgmShapeCollision.h"

#include "kgmGameTools.h"

class kgmIGame;

class kgmGameResources: public kgmObject, public kgmIResources
{
protected:
  struct Path
  {

    u32       type;
    kgmString  path;
    kgmFile    file;
    kgmArchive archive;

    Path()
    {
      type = 0;
    }

    ~Path()
    {
      path.clear();
      file.close();
      archive.close();
    }
  };

  kgmIGC*                m_gc;
  kgmIAudio*             m_audio;

  kgmList<kgmResource*>  m_resources;
  kgmList<Path*>         m_paths;
  kgmGameTools           m_tools;

public:
  kgmGameResources(kgmIGC* gc, kgmIAudio* audio);
  ~kgmGameResources();

  void               clear(u32 group);
  void               remove(kgmResource*);
  bool               exists(kgmResource*);

  void               addPath(kgmString s);
  bool               getFile(const char* id, kgmMemory<u8>& m);

  kgmPicture*        getPicture(const char* id);
  kgmTexture*        getTexture(const char* id);
  kgmShader*         getShader(const char* id);
  kgmAnimation*      getAnimation(const char* id);
  kgmSound*          getSound(const char*);
  kgmMesh*           getMesh(const char* id);
  kgmSkeleton*       getSkeleton(const char* id);
  kgmFont*           getFont(const char* id, u32 r, u32 c);
  kgmShapeCollision* getShapeCollision(const char* id);

private:
  void               add(kgmResource*);
  kgmResource*       get(const char* id);
  kgmResource*       get(kgmString& id);
};
