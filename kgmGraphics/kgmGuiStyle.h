#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmXml.h"
#include "kgmTexture.h"
#include "kgmFont.h"

/*
  This class used as GUI theme.
  Its not GUI, only for use GUI ctive parametes. as colors, textures and etc....
*/

class kgmGuiStyle: public kgmObject{
 public:
  typedef u32 color;
  typedef kgmTexture* image;
  typedef kgmFont* font;

  struct SGui{
    color tx_color;
    color bg_color;
    color fg_color;
    color ac_color;
    image bg_image;
  };

  struct SGuiButton: SGui{

  };

  struct SGuiList: SGui{

  };

  struct SGuiProgress: SGui{

  };
  //gui
  font  gui_font;
  
  SGui sgui;
  SGuiList slist;
  SGuiButton sbutton;
  SGuiProgress sprogess;
 public:
  kgmGuiStyle();
  kgmGuiStyle(kgmXml&);
};
