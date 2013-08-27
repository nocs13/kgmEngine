#pragma once
#include "kgmTypes.h"
#include "kgmInterface.h"

class kgmMesh;
class kgmAnimation;
class kgmSound;
class kgmShader;
class kgmTexture;
class kgmMaterial;
class kgmResource;
class kgmSkeleton;
class kgmPicture;
class kgmFont;
class kgmFile;
class kgmXml;
class kgmShapeCollision;

template<class T> class kgmMemory;

class kgmIResources: public kgmInterface
{
private:
  enum Type{
    TypeNone,
    TypeMemory,
    TypeFile,
    TypeMesh,
    TypeShader,
    TypeTexture,
    TypeMaterial,
  };

private:
  class _Resource{
    friend class kgmIResources;
    s32 references;
    kgmIResources*    resource_manager;
  protected:
    _Resource(kgmIResources* rs)
    {
      references = 1;
      resource_manager = rs;
    }

    virtual ~_Resource(){
    }

  public:
    void release(){
      if(references > 0)
        references--;

      if(references < 1)
        resource_manager->remove(this);
    }
  };

  friend class _Resource;

public:
  template <class T> class Resource: public _Resource{
    friend class kgmIResources;

    T*  resource;
    Resource(kgmIResources* rs, T* r)
    :_Resource(rs){
      resource = r;
    }

    virtual ~Resource(){
    }

  public:

    operator T*() const
    {
      return resource;
    }
  };

private:
  virtual void                remove(_Resource*){};

public:
  virtual void                add(kgmResource*) = 0;
  virtual void                remove(kgmResource*) = 0;

  virtual bool                getFile(char*, kgmMemory<char>&) = 0;
  virtual kgmPicture*         getPicture(char*) = 0;
  virtual kgmTexture*         getTexture(char*) = 0;
  virtual kgmMaterial*        getMaterial(char*) = 0;
  virtual kgmShader*          getShader(char*) = 0;
  virtual kgmAnimation*       getAnimation(char*) = 0;
  virtual kgmSkeleton*        getSkeleton(char*) = 0;
  virtual kgmSound*           getSound(char*) = 0;
  virtual kgmMesh*            getMesh(char*) = 0;
  virtual kgmFont*            getFont(char*, u32 r, u32 c) = 0;
  virtual kgmShapeCollision*  getShapeCollision(char* id) = 0;
};
