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
  typedef u32         Color;
  typedef kgmTexture* Image;
  typedef kgmFont*    Font;

  struct SGui{
    Color tx_color; // Text color
    Color bg_color; // Background color
    Color fg_color; // Foreground color
    Color ac_color; // Active color
    Image image;    // Background image
  };

  struct SGuiButton: SGui{

  };

  struct SGuiList: SGui{

  };

  struct SGuiProgress: SGui{

  };

  //gui
  Font  gui_font;
  
  SGui sgui;
  SGuiList slist;
  SGuiButton sbutton;
  SGuiProgress sprogess;

 public:
  kgmGuiStyle();
  kgmGuiStyle(kgmXml&);
};
