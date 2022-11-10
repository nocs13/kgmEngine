#pragma once
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmArray.h"
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
#include "../kgmPhysics/kgmCollision.h"
#include "../kgmPhysics/kgmShapeCollision.h"

class kgmPicture;
class kgmUnit;
class kgmGameMap;
class kgmParticles;
class kgmIPhysics;
class kgmBody;
class kgmIGame;
class kgmGui;
class kgmGameScript;

class kgmGameTools
{
public:
 kgmGameTools();
 virtual ~kgmGameTools();
//  DRAWING
// static void gcDrawRect(kgmIGC* gc, int x, int y, int w, int h, u32 col, void* tex);
// static void gcDrawText(kgmIGC* gc, kgmFont* font, int fw, int fh, int x, int y, int w, int h, u32 col, kgmString& text);

//  PICTURE & TEXTURE & FONT & STYLE
 static kgmPicture*   genPicture(kgmArray<u8>& m);
 static kgmPicture*   genPictureFromBmp(kgmArray<u8>& m);
 static kgmPicture*   genPictureFromTga(kgmArray<u8>& m);
 static kgmTexture*   genTexture(kgmIGC* gc, kgmArray<u8>& m);
 static kgmFont*      genFont(kgmIGC* gc, u32 w, u32 h, u32 r, u32 c, kgmArray<u8>& m);
 static kgmGuiStyle*  genGuiStyle(kgmIResources* rc, kgmString id);

// SHADER & MATERIAL
 static kgmMaterial*  genMaterial(kgmArray<u8>& m);
 static kgmMaterial*  genMaterial(kgmXml& x);
 static kgmShader*    genShader(kgmIGC* gc, kgmString& s);
 static kgmShader*    genShader(kgmIGC* gc, kgmXml& s);
 static kgmShader*    genShader(kgmIGC* gc, kgmArray<u8>&, kgmArray<u8>&);

//////////// SKELETON
 static kgmSkeleton*  genSkeleton(kgmArray<u8>& m);
 static kgmSkeleton*  genSkeleton(kgmXml& x);

//ANIMATION
 static kgmAnimation* genAnimation(kgmArray<u8>& m);
 static kgmAnimation* genAnimation(kgmXml& x);

//MESHES
 static kgmMesh*  genMesh(kgmArray<u8>& m);
 static kgmMesh*  genMesh(kgmXml& x);

//SHAPES
  static s32                  genShapeCollision(kgmXml& x, kgmList<triangle3>& shape);
  static kgmShapeCollision*   genShapeCollision(kgmXml& x);
  static kgmCollision::Shape* genShapeCollision(kgmMesh& x);

//SOUNDS
 static kgmSound* genSound(kgmIAudio* snd, kgmArray<u8>& m);

 //PARTICLES
 static kgmParticles* genParticles(kgmXml& x);

 //GUI
 static kgmGui* genGui(kgmGameScript*, kgmXml& x);

 //PARSING TOOLS
 static kgmMesh*     parseMesh(kgmXml::Node& node);
 static kgmLight*    parseLight(kgmXml::Node& node);
 static kgmMaterial* parseMaterial(kgmXml::Node& node);
 static kgmSkeleton* parseSkeleton(kgmXml::Node& node);

 //UNIT TOOLS
 static bool initActor(kgmIGame* g, kgmUnit *a, kgmXml &xml);
 static bool initActor(kgmIGame* g, kgmUnit *a, kgmString id);
};
