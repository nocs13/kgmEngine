#pragma once
#include "kgmInterface.h"

typedef unsigned int uint;

class kgmMesh;
class kgmAnimation;
class kgmSound;
class kgmShader;
class kgmTexture;
class kgmMaterial;
class kgmResource;
class kgmSkeleton;
class kgmFont;
class kgmPicture;

template<class T> class kgmMemory;

class kgmIResources: public kgmInterface
{
public:

public:
 virtual void		add(kgmResource*) = 0;
 virtual void		remove(kgmResource*) = 0;

 virtual bool				  getFile(char*, kgmMemory<char>&) = 0;
 virtual kgmPicture*          getPicture(char*) = 0;
 virtual kgmTexture*          getTexture(char*) = 0;
 virtual kgmMaterial*         getMaterial(char*) = 0;
 virtual kgmShader*           getShader(char*) = 0;
 virtual kgmAnimation*        getAnimation(char*) = 0;
 virtual kgmSound*            getSound(char*) = 0;
 virtual kgmMesh*             getMesh(char*) = 0;
 virtual kgmSkeleton*         getSkeleton(char*) = 0;
 virtual kgmFont*             getFont(char*, uint r, uint c) = 0;
};
