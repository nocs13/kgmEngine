#include "kgmGuiStyle.h"

kgmGuiStyle::kgmGuiStyle()
{
  //gui
  sgui.tx_color = 0xffffffff;
  sgui.bg_color = 0xff999999;
  sgui.ac_color = 0xffcccccc;
  sgui.fg_color = 0xffaaaaaa;
  sgui.ft_size  = 20;

  //gui button
  sbutton.tx_color = 0xffffffff;
  sbutton.bg_color = 0xff999999;
  sbutton.fg_color = 0xffbbbbbb;
  sbutton.ac_color = 0xffcccccc;

  //gui list
  slist.tx_color = 0xffffffff;
  slist.bg_color = 0xffaaaaaa;
  slist.ac_color = 0xffaaaaaa;
  slist.fg_color = 0xffbbbbbb;

  //gui menu
  smenu.tx_color = 0xffffffff;
  smenu.bg_color = 0xffaaaaaa;
  smenu.fg_color = 0xffbbbbbb;
  smenu.ac_color = 0xff999999;

  //gui text
  stext.tx_color = 0xffffffff;
  stext.bg_color = 0xffaaaaaa;
  stext.ac_color = 0xffaaaaaa;
  stext.fg_color = 0xffbbbbbb;
  stext.cr_color = 0xff888888;
  stext.ft_size  = 18;

  //gui label
  slabel.tx_color = 0xffffffff;
  slabel.bg_color = 0xff999999;
  slabel.ac_color = 0xffaaaaaa;
  slabel.fg_color = 0xffbbbbbb;

  //gui check
  scheck.tx_color = 0xffffffff;
  scheck.bg_color = 0xff999999;
  scheck.ac_color = 0xffaaaaaa;
  scheck.fg_color = 0xffbbbbbb;
  scheck.bg_check = 0xff555555;
  scheck.fg_check = 0xffeeeeee;

  //gui scroll
  sscroll.tx_color = 0xffffffff;
  sscroll.bg_color = 0xff999999;
  sscroll.ac_color = 0xffaaaaaa;
  sscroll.fg_color = 0xffbbbbbb;

  gui_font  = null;
  gui_image = null;

  gui_border = 0xffffffff;
}

kgmGuiStyle::~kgmGuiStyle()
{
}
