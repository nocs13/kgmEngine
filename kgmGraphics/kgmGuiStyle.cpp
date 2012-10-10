#include "kgmGuiStyle.h"

kgmGuiStyle::kgmGuiStyle()
{
  //gui
  gui_font = 0;
  
  sgui.tx_color = 0xff000000;
  sgui.bg_color = 0xffffffff;
  sgui.ac_color = 0xff888888;
  sgui.fg_color= 0xffaaaaaa;
  sgui.bg_image = 0;

  //gui button
  sbutton.tx_color = 0xff000000;
  sbutton.bg_color = 0xff999999;
  sbutton.fg_color = 0xffbbbbbb;
  sbutton.ac_color = 0xffdddddd;
  sbutton.bg_image = 0;

  //gui list
  slist.tx_color = 0xff000000;
  slist.bg_color = 0xffaaaaaa;
  slist.ac_color = 0xffaaaaaa;
  slist.fg_color = 0xffbbbbbb;
  slist.bg_image = 0;

}

kgmGuiStyle::kgmGuiStyle(kgmXml& x)
{

}
