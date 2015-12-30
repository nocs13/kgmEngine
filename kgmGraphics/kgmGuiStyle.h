#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmBase/kgmXml.h"
#include "kgmTexture.h"
#include "kgmFont.h"

/*
  This class used as GUI theme.
  Its not GUI, only for use GUI ctive parametes. as colors, textures and etc....
*/

class kgmGuiStyle: public kgmObject
{
 public:
  typedef u32         Color;
  typedef kgmTexture* Image;
  typedef kgmFont*    Font;

  struct SGui
  {
    Color tx_color; // Text color
    Color bg_color; // Background color
    Color fg_color; // Foreground color
    Color ac_color; // Active color
    u32   ft_size;  // Font height

    //kgm_ptr<kgmTexture> image;    // Background image
    Image image;    // Background image
  };

  struct SGuiButton: SGui
  {

  };

  struct SGuiList: SGui
  {

  };

  struct SGuiText: SGui
  {
    Color cr_color;  //Cursor color
  };

  struct SGuiMenu: SGui
  {

  };

  struct SGuiLabel: SGui
  {

  };

  struct SGuiCheck: SGui
  {
    Color bg_check;
    Color fg_check;
  };

  struct SGuiScroll: SGui
  {

  };

  struct SGuiProgress: SGui
  {

  };

  //gui
  //kgm_ptr<kgmFont>  gui_font;
  Font  gui_font;

  SGui         sgui;
  SGuiList     slist;
  SGuiMenu     smenu;
  SGuiText     stext;
  SGuiLabel    slabel;
  SGuiCheck    scheck;
  SGuiScroll   sscroll;
  SGuiButton   sbutton;
  SGuiProgress sprogess;

 public:
  kgmGuiStyle();
  ~kgmGuiStyle();
};
