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
class kgmFont;
class kgmFile;
class kgmXml;
class kgmShapeCollision;

template<class T> class kgmMemory;

class kgmIResources
{
public:
  virtual void                remove(kgmResource*) = 0;

  virtual bool                getFile(char*, kgmMemory<u8>&) = 0;
  virtual kgmPicture*         getPicture(char*) = 0;
  virtual kgmTexture*         getTexture(char*) = 0;
  virtual kgmShader*          getShader(char*) = 0;
  virtual kgmAnimation*       getAnimation(char*) = 0;
  virtual kgmSkeleton*        getSkeleton(char*) = 0;
  virtual kgmSound*           getSound(char*) = 0;
  virtual kgmMesh*            getMesh(char*) = 0;
  virtual kgmFont*            getFont(char*, u32 r, u32 c) = 0;
  virtual kgmShapeCollision*  getShapeCollision(char* id) = 0;
};
