#pragma once
#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmMap.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmArchive.h"
#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmArray.h"
#include "../kgmMedia/kgmSound.h"
#include "../kgmGraphics/kgmAnimation.h"
#include "../kgmGraphics/kgmPicture.h"
#include "../kgmGraphics/kgmTexture.h"
#include "../kgmGraphics/kgmShader.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmPhysics/kgmShapeCollision.h"

#include "../kgmGame/kgmGameTools.h"

class kgmResources: public kgmObject, public kgmIResources
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
  //kgmTab<const char*, kgmResource*> m_resources;
  //kgmMap<const char*, kgmResource*> m_resources;
  kgmList<Path*>         m_paths;
  kgmGameTools           m_tools;


public:
  static kgmIResources* generate(kgmIGC* gc, kgmIAudio* audio);

  kgmResources(kgmIGC* gc, kgmIAudio* audio);
  ~kgmResources();

  void               reset();
  void               clear();
  void               add(kgmResource*);
  void               remove(kgmResource*);
  bool               exists(kgmResource*);

  void               addPath(kgmString s);
  bool               getFile(const char* id, kgmArray<u8>& m);

  bool               getRFile(const char* id, const char *type, kgmArray<u8>& m);

  kgmPicture*        getPicture(const char* id);
  kgmTexture*        getTexture(const char* id);
  kgmShader*         getShader(const char* id);
  kgmMaterial*       getMaterial(const char*);
  kgmAnimation*      getAnimation(const char* id);
  kgmSound*          getSound(const char*);
  kgmMesh*           getMesh(const char* id);
  kgmSkeleton*       getSkeleton(const char* id);
  kgmFont*           getFont(const char* id, u32 r, u32 c);
  kgmShapeCollision* getShapeCollision(const char* id);

private:
  kgmResource*       get(const char* id);
  kgmResource*       get(kgmString& id);

  bool               fileFromData(kgmArray<u8>& mem, kgmString path);
  bool               dataFromFile(kgmString path, kgmArray<u8>& mem);
};
