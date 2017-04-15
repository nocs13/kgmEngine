#pragma once
#include "kgmTypes.h"

class kgmMesh;
class kgmAnimation;
class kgmSound;
class kgmShader;
class kgmTexture;
class kgmResource;
class kgmSkeleton;
class kgmPicture;
class kgmMaterial;
class kgmFont;
class kgmFile;
class kgmXml;
class kgmShapeCollision;

template<class T> class kgmMemory;

class kgmIResources
{
protected:
  void lock(kgmResource* r);
  void unlock(kgmResource* r);

public:
  virtual void                clear() = 0;
  virtual void                remove(kgmResource*) = 0;
  virtual bool                exists(kgmResource*) = 0;

  virtual bool                getFile(const char*, kgmMemory<u8>&) = 0;
  virtual kgmPicture*         getPicture(const char*) = 0;
  virtual kgmTexture*         getTexture(const char*) = 0;
  virtual kgmShader*          getShader(const char*) = 0;
  virtual kgmMaterial*        getMaterial(const char*) = 0;
  virtual kgmAnimation*       getAnimation(const char*) = 0;
  virtual kgmSkeleton*        getSkeleton(const char*) = 0;
  virtual kgmSound*           getSound(const char*) = 0;
  virtual kgmMesh*            getMesh(const char*) = 0;
  virtual kgmFont*            getFont(const char*, u32 r, u32 c) = 0;
  virtual kgmShapeCollision*  getShapeCollision(const char* id) = 0;
};
