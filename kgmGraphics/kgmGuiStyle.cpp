#include "kgmGuiStyle.h"

kgmGuiStyle::kgmGuiStyle()
{
  //gui
  gui_font = null;
  
  sgui.tx_color = 0xffffffff;
  sgui.bg_color = 0xffffffff;
  sgui.ac_color = 0xffffffff;
  sgui.fg_color = 0xffffffff;
  sgui.image    = null;

  //gui button
  sbutton.tx_color = 0xffffffff;
  sbutton.bg_color = 0xff999999;
  sbutton.fg_color = 0xffbbbbbb;
  sbutton.ac_color = 0xffdddddd;
  sbutton.image    = null;

  //gui list
  slist.tx_color = 0xffffffff;
  slist.bg_color = 0xffaaaaaa;
  slist.ac_color = 0xffaaaaaa;
  slist.fg_color = 0xffbbbbbb;
  slist.image    = null;

}

kgmGuiStyle::kgmGuiStyle(kgmXml& x)
{
  if(!x.m_node)
    return;

  for(int i = 0; i < x.m_node->m_nodes.size(); i++)
  {
    kgmXml::Node* n = x.m_node->m_nodes[i];

    if(n->m_name == "Font")
    {

    }
    else if(n->m_name == "Gui")
    {

    }
    else if(n->m_name == "GuiList")
    {

    }
    else if(n->m_name == "GuiButton")
    {

    }
  }
}
