#pragma once

#include "../../kgmBase/kgmIGC.h"

#include "../kgmGui.h"
#include "../kgmFont.h"
#include "../kgmTexture.h"

class kgmIGC;
class kgmGui;
class kgmIcon;
class kgmFont;
class kgmMesh;
class kgmSprite;
class kgmVisual;
class kgmTexture;
class kgmParticles;

void gcDrawRect(kgmIGC* gc, kgmGui::Rect rc, u32 col, kgmTexture* tex);

void gcDrawText(kgmIGC* gc, kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor, kgmGui::Rect clip, kgmString& text);

void gcDrawBillboard(kgmIGC* gc, box b, u32 col);

//void gcDraw(kgmGui*);
void gcDraw(kgmIGC*, kgmMesh*);
void gcDraw(kgmIGC*, kgmSprite*);
void gcDraw(kgmIGC*, kgmParticles*);
void gcDraw(kgmIGC*, kgmIcon*);
