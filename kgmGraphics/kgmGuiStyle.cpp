#include "kgmGuiStyle.h"

kgmGuiStyle::kgmGuiStyle()
{
  //gui
  gui_font = null;
  
  sgui.tx_color = 0xffffffff;
  sgui.bg_color = 0xffffffff;
  sgui.ac_color = 0xffffffff;
  sgui.fg_color = 0xffffffff;
  sgui.ft_size  = 20;
  sgui.image    = null;

  //gui button
  sbutton.tx_color = 0xffffffff;
  sbutton.bg_color = 0xff999999;
  sbutton.fg_color = 0xffbbbbbb;
  sbutton.ac_color = 0xffbbbbbb;
  sbutton.image    = null;

  //gui list
  slist.tx_color = 0xffffffff;
  slist.bg_color = 0xffaaaaaa;
  slist.ac_color = 0xffaaaaaa;
  slist.fg_color = 0xffbbbbbb;
  slist.image    = null;

  //gui menu
  smenu.tx_color = 0xffffffff;
  smenu.bg_color = 0xffaaaaaa;
  smenu.ac_color = 0xffaaaaaa;
  smenu.fg_color = 0xffbbbbbb;
  smenu.image    = null;

  //gui text
  stext.tx_color = 0xffffffff;
  stext.bg_color = 0xffaaaaaa;
  stext.ac_color = 0xffaaaaaa;
  stext.fg_color = 0xffbbbbbb;
  stext.cr_color = 0xff888888;
  stext.ft_size  = 18;
  stext.image    = null;

  //gui label
  slabel.tx_color = 0xffffffff;
  slabel.bg_color = 0xff999999;
  slabel.ac_color = 0xffaaaaaa;
  slabel.fg_color = 0xffbbbbbb;
  slabel.image    = null;

  //gui scroll
  sscroll.tx_color = 0xffffffff;
  sscroll.bg_color = 0xff999999;
  sscroll.ac_color = 0xffaaaaaa;
  sscroll.fg_color = 0xffbbbbbb;
  sscroll.image    = null;
}
