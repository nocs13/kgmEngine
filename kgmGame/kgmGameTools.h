#pragma once
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmMemory.h"
#include "../kgmMedia/kgmSound.h"
#include "../kgmMedia/kgmWave.h"
#include "../kgmGraphics/kgmFont.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmPolygon.h"
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmAnimation.h"
#include "../kgmGraphics/kgmSkeleton.h"
#include "../kgmGraphics/kgmGuiStyle.h"
#include "../kgmPhysics/kgmShapeCollision.h"

class kgmPicture;
class kgmActor;
class kgmGameMap;
class kgmParticles;

class kgmGameTools{
public:
 kgmGameTools();
 virtual ~kgmGameTools();
//  DRAWING 
// static void gcDrawRect(kgmIGC* gc, int x, int y, int w, int h, u32 col, void* tex);
// static void gcDrawText(kgmIGC* gc, kgmFont* font, int fw, int fh, int x, int y, int w, int h, u32 col, kgmString& text);

//  PICTURE & TEXTURE & FONT & STYLE
 static kgmPicture*   genPicture(kgmMemory<char>& m);
 static kgmPicture*   genPictureFromBmp(kgmMemory<char>& m);
 static kgmPicture*   genPictureFromTga(kgmMemory<char>& m);
 static kgmTexture*   genTexture(kgmIGC* gc, kgmMemory<char>& m);
 static kgmFont*      genFont(kgmIGC* gc, u32 w, u32 h, u32 r, u32 c, kgmMemory<char>& m);
 static kgmGuiStyle*  genGuiStyle(kgmIResources* rc, kgmString id);

// SHADER & MATERIAL
 static kgmMaterial*  genMaterial(kgmMemory<char>& m);
 static kgmMaterial*  genMaterial(kgmXml& x);
 static kgmShader*    genShader(kgmIGC* gc, kgmString& s);
 static kgmShader*    genShader(kgmIGC* gc, kgmXml& s);

//////////// SKELETON
 static kgmSkeleton*  genSkeleton(kgmMemory<char>& m);
 static kgmSkeleton*  genSkeleton(kgmXml& x);

//ANIMATION
 static kgmAnimation* genAnimation(kgmMemory<char>& m);
 static kgmAnimation* genAnimation(kgmXml& x);

//MESHES
 static kgmMesh*  genMesh(kgmMemory<char>& m);
 static kgmMesh*  genMesh(kgmXml& x);

 //SHAPES
  static s32                 genShapeCollision(kgmXml& x, kgmList<triangle3>& shape);
  static kgmShapeCollision*  genShapeCollision(kgmXml& x);

//SOUNDS
 static kgmSound* genSound(kgmIAudio* snd, kgmMemory<char>& m);

 //PARTICLES
 static kgmParticles* genParticles(kgmXml& x);
};
